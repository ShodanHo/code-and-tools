#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <errno.h>
#include <getopt.h>
#include "mqtt_subscriber.h"

static void subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count,
                               const int *granted_qos)
{
  std::cout << "Subscribed to mid " << mid << " with qos " << granted_qos[0];
  for (int i = 1; i < qos_count; i++) {
    std::cout << ", " << granted_qos[i];
  }
  std::cout << std::endl;
}

int run_subscriber(int argc, char* const argv[], void *data_ptr,
                   std::string hostname, int port, unsigned num_threads, std::list<TopicQos> topicQoses,
                   void (*message_callback)(struct mosquitto *mosq, void *obj,
                       const struct mosquitto_message *message))
{
  mosquitto_lib_init();

  //std::string hostname("localhost");
  //std::list<std::string> topics;
  //int qos = 0; // Cheap.
  //int port = 1883;
  //int num_threads = 1;

  enum {
    HELP_OPTION = '?',
    //HOST_OPTION = 'h',
    //TOPIC_OPTION = 't',
    //QOS_OPTION = 'q',
    PORT_OPTION = 'p',
    //PARALLEL_OPTION = 'P',
  };
  struct option options[] = {
    {"help", 0, nullptr, HELP_OPTION},
    //{"mqtt-host", 1, nullptr, HOST_OPTION},
    //{"topic", 1, nullptr, TOPIC_OPTION},
    //{"qos", 1, nullptr, QOS_OPTION},
    {"mqtt-port", 1, nullptr, PORT_OPTION},
    //{"parallel", 1, nullptr, PARALLEL_OPTION},
    {0}
  };

  bool more_options = true;
  while (more_options) {
    int status = getopt_long(argc, argv, "h:t:q:p:j", options, nullptr);

    switch (status) {
#if 0
      case HOST_OPTION:
      hostname = optarg;
      break;

    case TOPIC_OPTION:
      topics.push_back(optarg);
      break;
#endif

    case HELP_OPTION:
      exit(EXIT_FAILURE);
      break;

#if 0
    case QOS_OPTION:
      qos = atoi(optarg);
      break;

    case PORT_OPTION:
      port = atoi(optarg);
      break;

      case PARALLEL_OPTION:
      num_threads = atoi(optarg);
      break;
#endif

    default:
      more_options = false;
      break;
    }
  }

  std::cout << "Connect to host " << hostname << " on port " << port << std::endl;
  for (auto& topicQos : topicQoses) {
    std::cout << "Subscribe under topic " << topicQos.topic << " quality of service of " << topicQos.qos << std::endl;
  }

  {
    std::list<struct mosquitto*> mosq_list;

    for (unsigned i = 0; i < num_threads; i++) {
      struct mosquitto *mosq = mosquitto_new(nullptr, /*clean_session=*/true, data_ptr);
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

    for (auto mosq : mosq_list) {
      mosquitto_message_callback_set(mosq, message_callback);
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

    for (auto& topicQos : topicQoses) {
      int mid;
      struct mosquitto* mosq = mosq_list.front();

      // Transfer to back of list.
      mosq_list.pop_front();
      mosq_list.push_back(mosq);
      
      int code = mosquitto_subscribe(mosq, &mid, topicQos.topic.c_str(), topicQos.qos);
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
      std::cout << "Subscribing to topic " << topicQos.topic << " with mid " << mid << std::endl;
    }

    for (auto mosq : mosq_list) {
      //      mosquitto_disconnect(mosq);
      mosquitto_loop_stop(mosq, false);
      mosquitto_destroy(mosq);
    }
  }

  mosquitto_lib_cleanup();
  return 0;
}
