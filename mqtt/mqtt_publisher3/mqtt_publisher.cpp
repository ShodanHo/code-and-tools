/******************************************************
DESCRIPTION:
Publish MQTT messages received on a list.
*/

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <mosquitto.h>
#include "mqtt_publisher.h"

std::string
TopicPayloadQos::toString(void) const
{
  std::ostringstream oss;
  oss << '(' << topic << ',' << payload << ',' << qos << ')';
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const TopicPayloadQos& tpq)
{
  os << tpq.toString();
  return os;
}

static void publish_some_data(struct mosquitto *mosq,
                              std::string topic,
                              std::string payload,
                              int qos)
{
  int mid;
  int code = mosquitto_publish(mosq, &mid, topic.c_str(),
                               payload.size(), payload.c_str(), qos, /*(retain=*/false);
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
}

int run_mqtt_publisher(const std::string& hostname, int port,
                       conditioned_list<TopicPayloadQos>* input_list)
{
  mosquitto_lib_init();

  std::cout << "MQTT publisher" << std::endl;

  std::cout << "Connect to host " << hostname << " on port " << port << std::endl;

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

    for (bool quit = false; !quit;) { // quit when QOS < 0
      input_list->cond_wait(); // wait for input

      // process all input
      while (input_list->size() > 0)
      {
        auto topicPayloadQos = input_list->front_value();
        input_list->pop_front();
        std::cout << "topicPayloadQos=" << topicPayloadQos << '\n';
        if (topicPayloadQos.qos < 0) {
          quit = true;
          break;
        }
        publish_some_data(mosq, topicPayloadQos.topic, topicPayloadQos.payload, topicPayloadQos.qos);
      }
    }

    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, false);

    mosquitto_destroy(mosq);
  }

  mosquitto_lib_cleanup();
  return 0;
}
