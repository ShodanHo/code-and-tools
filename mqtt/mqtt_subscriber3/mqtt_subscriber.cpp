#include "mqtt_subscriber.h"
#include <sstream> // ostringstream
#include <cstring> // strerror
#include <errno.h> // errno

std::string
TopicQos::toString(void) const
{
  std::ostringstream oss;
  oss << '(' << topic << ',' << qos << ')';
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const TopicQos& tq)
{
  os << tq.toString();
  return os;
}

static void subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count,
                               const int *granted_qos)
{
  std::cout << "Subscribed to mid " << mid << " with qos " << granted_qos[0];
  for (int i = 1; i < qos_count; i++) {
    std::cout << ", " << granted_qos[i];
  }
  std::cout << std::endl;
}

int run_subscriber(void *data_ptr,
                   const std::string* hostname, int port, unsigned num_threads,
                   std::list<TopicQos>* topicQoses,
                   void (*message_callback)(struct mosquitto *mosq, void *obj,
                       const struct mosquitto_message *message))
{
  mosquitto_lib_init();

  std::cout << "Connect to host " << hostname << " on port " << port << std::endl;
  for (auto& topicQos : *topicQoses) {
    std::cout << "Subscribe under topic " << topicQos.topic << " quality of service of " << topicQos.qos << std::endl;
  }

  {
    std::list<struct mosquitto*> mosq_list;

    for (unsigned i = 0; i < num_threads; i++) {
      struct mosquitto *mosq = mosquitto_new(nullptr, /*clean_session=*/true, data_ptr);
      int code = mosquitto_connect(mosq, hostname->c_str(), port, /*keepalive=*/-1);
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

    for (auto& topicQos : *topicQoses) {
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
