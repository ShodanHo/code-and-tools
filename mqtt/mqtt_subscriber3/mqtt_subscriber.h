/*
 * mqtt_subscriber.h
 *
 *  Created on: 19/02/2016
 *      Author: stuartf
 */

#ifndef MQTT_SUBSCRIBER_CPP_MQTT_SUBSCRIBER_H_
#define MQTT_SUBSCRIBER_CPP_MQTT_SUBSCRIBER_H_

#include "mosquitto.h"

struct TopicQos
{
  std::string topic;
  int qos;
  TopicQos(decltype(topic) _topic,
           decltype(qos) _qos)
  : topic(_topic)
  , qos(_qos) {}
};

int run_subscriber(int argc, char* const argv[], void *data_ptr,
                   std::string hostname, int port, unsigned num_threads, std::list<TopicQos> topicQoses,
                   void (*message_callback)(struct mosquitto *mosq, void *obj,
                       const struct mosquitto_message *message));

#endif /* MQTT_SUBSCRIBER_CPP_MQTT_SUBSCRIBER_H_ */
