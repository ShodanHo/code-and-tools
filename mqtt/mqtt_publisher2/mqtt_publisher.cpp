/******************************************************
FILE NAME: tx_spectrum/main.cpp
DESCRIPTION:
Publish MQTT messages in binary format, simulating spectrum data.
The first (little endian) 64 bit integer is the system clock nanosecond count since the epoch.
The rest of the data are random 32bit floats.
*/

#include <iostream>
#include <sstream>
#include <random>
#include <ctime>
#include <cstring>
#include <thread>
#include <chrono>

#include <getopt.h>
#include <mosquitto.h>
#include <boost/format.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define DISABLE_NAGLE 0

#define SEC_TO_NSEC 1000000000ULL

bool get_timestamp(uint64_t &nanosec)
{
  struct timespec ts;
  if (0 != clock_gettime(CLOCK_REALTIME, &ts)) {
    return false;
  }

  nanosec = ts.tv_nsec + SEC_TO_NSEC*ts.tv_sec;
  return true;
}

void publish_some_data(struct mosquitto *mosq,
		       std::string topic,
		       int bins,
		       size_t element_size,
		       int qos)
{
  static std::default_random_engine generator;
  static std::uniform_real_distribution<float> distribution(0.0, 1.0);

  size_t payload_size = sizeof(uint64_t) + bins*element_size;
  char* payload = new char[payload_size];

  uint64_t* nsec = reinterpret_cast<uint64_t*>(payload);
  float* data = reinterpret_cast<float*>(nsec + 1);

  get_timestamp(*nsec);
  for (int i = 0; i < bins; i++) {
    data[i] = distribution(generator);
  }

  //  std::cout << "Publishing " << payload_size << " bytes..." << std::endl;
  int mid;
  int code = mosquitto_publish(mosq, &mid, topic.c_str(), payload_size, payload, qos, /*(retain=*/false);
  if (code != MOSQ_ERR_SUCCESS) {
    switch (code) {
    case MOSQ_ERR_INVAL:
      std::cerr << "Mosquitto publish failure - invalid input parameters" << std::endl;
	  break;
    case MOSQ_ERR_NOMEM:
      std::cerr << "Mosquitto publish failure - out of memory" << std::endl;
      break;
    case MOSQ_ERR_NO_CONN:
      std::cerr << "Mosquitto publish failure - no connection" << std::endl;
      break;
    case MOSQ_ERR_PROTOCOL:
      std::cerr << "Mosquitto publish failure - protocol error" << std::endl;
      break;
    case MOSQ_ERR_PAYLOAD_SIZE:
      std::cerr << "Mosquitto publish failure - Excessive payload" << std::endl;
      break;
    default:
      std::cerr << "Mosquitto publish failure - unknown error" << std::endl;
      break;
    }
    exit(EXIT_FAILURE);
  }

  //  std::cout << "Published" << std::endl;

  delete payload;
}

int run_publisher(int argc, char* const argv[])
{
  mosquitto_lib_init();
#if 0
  {
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    std::cout << "Mosquitto library version - " << major << "." << minor << "." << revision << std::endl;
  }
#endif
  std::cout << "Tx Spectrum test program" << std::endl;


  std::string hostname("localhost");
  std::string topic;
  float rate = 20.0; // Hz
  int bins = 4096;
  size_t element_size = sizeof(float);
  int qos = 0; // Cheap.
  int port = 1883;

  enum {
    HELP_OPTION = '?',
    HOST_OPTION = 'h',
    TOPIC_OPTION = 't',
    RATE_OPTION = 'R',
    BINS_OPTION = 'B',
    QOS_OPTION = 'q',
    PORT_OPTION = 'p',
  };
  struct option options[] = {
    {"help", 0, nullptr, HELP_OPTION},
    {"mqtt-host", 1, nullptr, HOST_OPTION},
    {"topic", 1, nullptr, TOPIC_OPTION},
    {"rate", 1, nullptr, RATE_OPTION},
    {"bins", 1, nullptr, BINS_OPTION},
    {"qos", 1, nullptr, QOS_OPTION},
    {"mqtt-port", 1, nullptr, PORT_OPTION},
    {0}
  };

  bool more_options = true;
  while (more_options) {
    int status = getopt_long(argc, argv, "h:t:R:B:q:p:", options, nullptr);

    switch (status) {
    case HOST_OPTION:
      hostname = optarg;
      break;

    case TOPIC_OPTION:
      topic = optarg;
      break;

    case HELP_OPTION:
      exit(EXIT_FAILURE);
      break;

    case RATE_OPTION:
      rate = atof(optarg);
      break;

    case BINS_OPTION:
      bins = atoi(optarg);
      break;

    case QOS_OPTION:
      qos = atoi(optarg);
      break;

    case PORT_OPTION:
      port = atoi(optarg);
      break;

    default:
      more_options = false;
      break;
    }
  }

  std::cout << "Connect to host " << hostname << " on port " << port << std::endl;
  std::cout << "Publish under topic " << topic << std::endl;
  std::cout << "Publish at rate " << rate << " Hz" << std::endl;
  std::cout << "Publish with quality of service of " << qos << std::endl;
  std::cout << "Publish with bin count of " << bins << std::endl;
  std::cout << "Publish with element size of " << element_size << std::endl;

  {
    struct mosquitto *mosq = mosquitto_new(nullptr, /*clean_session=*/true, nullptr);

    {
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
    }
#if DISABLE_NAGLE
    {
      int sock = mosquitto_socket(mosq);
      if (sock >= 0) {
	int flag = 1;
	int result = setsockopt(sock,            /* socket affected */
				IPPROTO_TCP,     /* set option at TCP level */
				TCP_NODELAY,     /* name of option */
				(char *) &flag,  /* the cast is historical
						    cruft */
				sizeof(int));    /* length of option value */
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
    {
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

    uint64_t next_call_nsec;
    get_timestamp(next_call_nsec);

    uint64_t period_nsec = static_cast<uint64_t>(ceil(1e9/rate));
    
    for (;;) {
      publish_some_data(mosq, topic, bins, element_size, qos);
      next_call_nsec += period_nsec;
      uint64_t now_nsec;
      get_timestamp(now_nsec);
      std::this_thread::sleep_for(std::chrono::nanoseconds(next_call_nsec - now_nsec));
    }

    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, false);

    mosquitto_destroy(mosq);
  }

  mosquitto_lib_cleanup();
}
