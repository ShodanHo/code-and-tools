#ifndef __MQTT_PUBLISHER_H__
#define __MQTT_PUBLISHER_H__

#include <string>
#include "mutex_cond_collections.h"

struct TopicPayloadQos
{
  std::string topic;
  std::string payload;
  int qos;
  TopicPayloadQos(decltype(topic) _topic,
                  decltype(payload) _payload,
                  decltype(qos) _qos)
  : topic(_topic)
  , payload(_payload)
  , qos(_qos) {}
  std::string toString(void) const;
};

std::ostream& operator<<(std::ostream& os, const TopicPayloadQos& tpq);

int run_mqtt_publisher(const std::string* hostname, int port, conditioned_list<TopicPayloadQos>* input_list);

#endif // __MQTT_PUBLISHER_H__
