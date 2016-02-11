/******************************************************
FILE NAME: rx_mqtt/main.cpp
DESCRIPTION:
Subscribe to MQTT messages, and report statistics on each topic.
Messages with either be in binary or JSON format.
The JSON 'time' field is an ISO 9601 with timezone UTC and fractional seconds.
The JSON format is compatible with InfluxDB.
The binary format has a first (little endian) 64 bit integer with the nanosecond count since the epoch.
In both cases, the time is the system time the message was composed, and is used to calculate the latency.
*/

#include <iostream>
#include <sstream>
#include <random>
#include <ctime>
#include <cstring>
#include <cmath>
#include <thread>
#include <chrono>
#include <list>
#include <map>
#include <mutex>

#include <getopt.h>
#include <mosquitto.h>
#include <boost/format.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "Statistics.h"

#define DISABLE_NAGLE 0

#define REPORT_INTERVAL 10 // seconds

#define SEC_TO_NSEC 1000000000ULL
#define NSEC_TO_SEC 1e-9
#define NSEC_TO_MSEC 1e-6

struct PerTopicStats {
  common::statistics::SampleStatistics transport_time_stats;
  common::statistics::SampleDifferenceStatistics interval_stats;
  common::statistics::SampleStatistics size_stats;
};
struct ReceiveStats {
  std::mutex mutex;
  std::map<std::string, PerTopicStats> stats; // by topic
  uint64_t base_nsec;
  uint64_t last_report_nsec;
};

bool get_timestamp(uint64_t &nanosec)
{
  struct timespec ts;
  if (0 != clock_gettime(CLOCK_REALTIME, &ts)) {
    return false;
  }

  nanosec = ts.tv_nsec + SEC_TO_NSEC*ts.tv_sec;
  return true;
}

void subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
  //ReceiveStats* data_obj = reinterpret_cast<ReceiveStats*>(obj);

  std::cout << "Subscribed to mid " << mid << " with qos " << granted_qos[0];
  for (int i = 1; i < qos_count; i++) {
    std::cout << ", " << granted_qos[i];
  }
  std::cout << std::endl;
}

bool string_to_nsec(const std::string &str,
		    uint64_t* nsec)
{
    int yyyy, mm, dd, HH, MM;
    float SSfff;

    if (6 != sscanf(str.c_str(), "%d-%d-%dT%d:%d:%fZ", &yyyy, &mm, &dd, &HH, &MM, &SSfff)) {
      return false;
    }
      
    int SS = (int)std::floor(SSfff);
    double fff = SSfff - SS;

    std::tm ttm;
    ttm.tm_year = yyyy - 1900; // Year since 1900
    ttm.tm_mon = mm - 1; // Month since January 
    ttm.tm_mday = dd; // Day of the month [1-31]
    ttm.tm_hour = HH; // Hour of the day [00-23]
    ttm.tm_min = MM;
    ttm.tm_sec = SS;
    ttm.tm_isdst = 0;

    time_t ttime = ::timegm(&ttm);

    *nsec = ttime*SEC_TO_NSEC + static_cast<uint64_t>(fff*SEC_TO_NSEC);
    return true;
}

bool get_payload_timestamp_json(void* payload, size_t size, uint64_t* tx_nsec)
{
  boost::property_tree::ptree pt;
  std::string json(reinterpret_cast<char*>(payload), size);
  std::istringstream input(json);

  try {
    boost::property_tree::read_json(input, pt);
  }
  catch (boost::property_tree::json_parser_error& e) {
    return false;
  }
  
  try {
    std::string time_string = pt.get<std::string>("time");
    return string_to_nsec(time_string, tx_nsec);
  }
  catch (boost::property_tree::ptree_bad_path& e) {
    return false;
  }
  catch (boost::property_tree::ptree_bad_data& e) {
    return false;
  }
  return false;
}

bool get_payload_timestamp_binary(void* payload, size_t size, uint64_t* tx_nsec)
{
  if (size < sizeof(uint64_t)) {
    return false;
  }
  
  uint64_t* p = reinterpret_cast<uint64_t*>(payload);
  *tx_nsec = *p;
  return true;
}

void add_to_stats(ReceiveStats *data_obj, uint64_t rx_nsec, uint64_t tx_nsec, const char* topic, size_t size)
{
  if (tx_nsec > rx_nsec) {
    // Backwards!
    std::cerr << "Transmit time is in the future!" << std::endl;
  }
  else {
    uint64_t transmit_nsec = rx_nsec - tx_nsec;
    double receive_time = (rx_nsec - data_obj->base_nsec)*NSEC_TO_SEC;

    PerTopicStats& stats(data_obj->stats[topic]);

    stats.transport_time_stats.addSample(transmit_nsec*NSEC_TO_MSEC);
    stats.interval_stats.addSample(receive_time);
    stats.size_stats.addSample(size);
  }
}

void report_stats(ReceiveStats *data_obj, uint64_t rx_nsec)
{
  if (rx_nsec - data_obj->last_report_nsec >= REPORT_INTERVAL*SEC_TO_NSEC) {
    bool reported = false;

    for (auto& i : data_obj->stats) {
      std::string topic(i.first);
      PerTopicStats& stats(i.second);

      if (stats.transport_time_stats.isValid() &&
	  stats.interval_stats.isValid() &&
	  stats.size_stats.isValid()) {
	reported = true;

	std::cout << "Topic " << topic << std::endl;

	std::cout << "  Received " << stats.interval_stats.getCount()
		  << " messages at a rate of " << 1.0/stats.interval_stats.getMean()
		  << " Hz, with mean payload of " << stats.size_stats.getMean()
		  << " bytes, " << stats.size_stats.getSum()/stats.interval_stats.getSum()*1e-6
		  << " Mbytes/sec"
		  << std::endl;
	std::cout << "  Transport delay is " << stats.transport_time_stats.getMean()
		  << " +- " << stats.transport_time_stats.getStandardDeviation()
		  << " ms (" << stats.transport_time_stats.getMin()
		  << " .. " << stats.transport_time_stats.getMax()
		  << ")"
		  << std::endl;
	
	stats.interval_stats.clear();
	stats.transport_time_stats.clear();
	stats.size_stats.clear();
      }
    }

    if (reported) {
      data_obj->last_report_nsec = rx_nsec;
      std::cout << std::endl;
    }
  }
}

void message_callback_binary(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
  ReceiveStats* data_obj = reinterpret_cast<ReceiveStats*>(obj);

  uint64_t rx_nsec;
  get_timestamp(rx_nsec);

  std::lock_guard<std::mutex> guard(data_obj->mutex);
  
  uint64_t tx_nsec;
  get_payload_timestamp_binary(message->payload, message->payloadlen, &tx_nsec);

  add_to_stats(data_obj, rx_nsec, tx_nsec, message->topic, message->payloadlen);
  report_stats(data_obj, rx_nsec);
}

#define HERE() __FUNCTION__ << ':' << __LINE__ << ':'
#define STR(x) #x << '=' << x

void message_callback_json(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
  std::string payload((char *)message->payload,message->payloadlen);
  std::cout << HERE() << STR(message->topic) << ' ' << STR(payload) << '\n';
  ReceiveStats* data_obj = reinterpret_cast<ReceiveStats*>(obj);

  uint64_t rx_nsec;
  get_timestamp(rx_nsec);

  std::lock_guard<std::mutex> guard(data_obj->mutex);
  
  uint64_t tx_nsec;
  get_payload_timestamp_json(message->payload, message->payloadlen, &tx_nsec);

  add_to_stats(data_obj, rx_nsec, tx_nsec, message->topic, message->payloadlen);
  report_stats(data_obj, rx_nsec);
}


int run_subscriber(int argc, char* const argv[])
//int main(int argc, char* const argv[])
{
  mosquitto_lib_init();
#if 0
  {
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    std::cout << "Mosquitto library version - " << major << "." << minor << "." << revision << std::endl;
  }
#endif
  std::cout << "Rx Measurement test program" << std::endl;


  std::string hostname("localhost");
  std::list<std::string> topics;
  int qos = 0; // Cheap.
  int port = 1883;
  bool use_json = false;
  int num_threads = 1;

  enum {
    HELP_OPTION = '?',
    HOST_OPTION = 'h',
    TOPIC_OPTION = 't',
    QOS_OPTION = 'q',
    PORT_OPTION = 'p',
    JSON_MSG_OPTION = 'j',
    BINARY_MSG_OPTION = 'B',
    PARALLEL_OPTION = 'P',
  };
  struct option options[] = {
    {"help", 0, nullptr, HELP_OPTION},
    {"mqtt-host", 1, nullptr, HOST_OPTION},
    {"topic", 1, nullptr, TOPIC_OPTION},
    {"qos", 1, nullptr, QOS_OPTION},
    {"mqtt-port", 1, nullptr, PORT_OPTION},
    {"json", 0, nullptr, JSON_MSG_OPTION},
    {"binary", 0, nullptr, BINARY_MSG_OPTION},
    {"parallel", 1, nullptr, PARALLEL_OPTION},
    {0}
  };

  bool more_options = true;
  while (more_options) {
    int status = getopt_long(argc, argv, "h:t:q:p:j", options, nullptr);

    switch (status) {
    case HOST_OPTION:
      hostname = optarg;
      break;

    case TOPIC_OPTION:
      topics.push_back(optarg);
      break;

    case HELP_OPTION:
      exit(EXIT_FAILURE);
      break;

    case QOS_OPTION:
      qos = atoi(optarg);
      break;

    case PORT_OPTION:
      port = atoi(optarg);
      break;

    case JSON_MSG_OPTION:
      use_json = true;
      break;

    case BINARY_MSG_OPTION:
      use_json = false;
      break;

    case PARALLEL_OPTION:
      num_threads = atoi(optarg);
      break;

    default:
      more_options = false;
      break;
    }
  }

  std::cout << "Connect to host " << hostname << " on port " << port << std::endl;
  for (auto& topic : topics) {
    std::cout << "Subscribe under topic " << topic << std::endl;
  }
  std::cout << "Subscribe with quality of service of " << qos << std::endl;

  {
    std::list<struct mosquitto*> mosq_list;
    
    ReceiveStats data_obj;
    get_timestamp(data_obj.base_nsec);
    data_obj.last_report_nsec = data_obj.base_nsec;

    for (int i = 0; i < num_threads; i++) {
      struct mosquitto *mosq = mosquitto_new(nullptr, /*clean_session=*/true, &data_obj);
      int code = mosquitto_connect(mosq, hostname.c_str(), port, /*keepalive=*/-1);
      if (code != MOSQ_ERR_SUCCESS) {
	switch (code) {
	case MOSQ_ERR_INVAL:
	  std::cerr << "Mosquitto connect failure - invalid input parameters" << std::endl;
	  break;
	case MOSQ_ERR_ERRNO:
	  std::cerr << "Mosquitto connect failure - " << strerror(errno) << std::endl;
	  break;
	default:
	  std::cerr << "Mosquitto connect failure - unknown error" << std::endl;
	  break;
	}
	exit(EXIT_FAILURE);
      }
      mosq_list.push_back(mosq);
    }

#if DISABLE_NAGLE
    for (auto mosq : mosq_list) {
      int sock = mosquitto_socket(mosq);
      if (sock >= 0) {
	int flag = 1;
	int result = setsockopt(sock,
				IPPROTO_TCP,
				TCP_NODELAY,
				(char *) &flag,
				sizeof(flag));
	if (result < 0) {
	  std::cerr << "Unable to disable Nagle algorithm on Misquitto socket, " << strerror(errno) << std::endl;
	}
	else {
	  std::cout << "Disabled Nagle algorithm on Misquitto socket" << std::endl;
	}
      }
      else {
	  std::cerr << "Unable to disable Nagle algorithm on Misquitto, no socket" << std::endl;
      }
    }
#endif

    for (auto mosq : mosq_list) {
      if (use_json) {
	mosquitto_message_callback_set(mosq, &message_callback_json);
      }
      else {
	mosquitto_message_callback_set(mosq, &message_callback_binary);
      }
      mosquitto_subscribe_callback_set(mosq, &subscribe_callback);
    }

    for (auto mosq : mosq_list) {
      int code = mosquitto_loop_start(mosq);
      if (code != MOSQ_ERR_SUCCESS) {
	switch (code) {
	case MOSQ_ERR_INVAL:
	  std::cerr << "Mosquitto loop start failure - invalid input parameters" << std::endl;
	  break;
	case MOSQ_ERR_NOT_SUPPORTED:
	  std::cerr << "Mosquitto loop start failure - not supported" << std::endl;
	  break;
	default:
	  std::cerr << "Mosquitto loop start failure - unknown error" << std::endl;
	  break;
	}
	exit(EXIT_FAILURE);
      }
    }

    for (auto& topic : topics) {
      int mid;
      struct mosquitto* mosq = mosq_list.front();

      // Transfer to back of list.
      mosq_list.pop_front();
      mosq_list.push_back(mosq);
      
      int code = mosquitto_subscribe(mosq, &mid, topic.c_str(), qos);
      if (code != MOSQ_ERR_SUCCESS) {
	switch (code) {
	case MOSQ_ERR_INVAL:
	  std::cerr << "Mosquitto subscribe failure - invalid input parameters" << std::endl;
	  break;
	case MOSQ_ERR_NOMEM:
	  std::cerr << "Mosquitto subscribe failure - out of memory" << std::endl;
	  break;
	case MOSQ_ERR_NO_CONN:
	  std::cerr << "Mosquitto subscribe failure - no connection" << std::endl;
	  break;
	default:
	  std::cerr << "Mosquitto subscribe failure - unknown error" << std::endl;
	  break;
	}
	exit(EXIT_FAILURE);
      }
      std::cout << "Subscribing to topic " << topic << " with mid " << mid << std::endl;
    }

    for (auto mosq : mosq_list) {
      //      mosquitto_disconnect(mosq);
      mosquitto_loop_stop(mosq, false);
      mosquitto_destroy(mosq);
    }
  }

  mosquitto_lib_cleanup();
}
