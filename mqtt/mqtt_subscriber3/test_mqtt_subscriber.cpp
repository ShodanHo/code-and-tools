#include <iostream>
#include <mutex>
#include <list>
#include "mqtt_subscriber.h"
#include "common_utils.h"

#define HERE() __FUNCTION__ << '(' << __LINE__ << ')'

struct xyz
{
  std::mutex mutex;
  int count;
};

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
  xyz* data_obj = reinterpret_cast<xyz*>(obj);
  std::lock_guard<std::mutex> guard(data_obj->mutex);

  data_obj->count++;

  std::string payload(reinterpret_cast<char*>(message->payload),message->payloadlen);

  std::cout << HERE() << STR(message->topic) << ' '
      << STR(payload) << ' ' << STR(data_obj->count) << '\n';
}

int main(int argc, char **argv)
{
  xyz data; data.count = 0;
  std::string hostname("localname");
  int port = 1883;
  std::list<TopicQos> topicQoses;
  run_subscriber(argc, argv, &data, hostname, port, 1, topicQoses, message_callback);
  return 0;
}
