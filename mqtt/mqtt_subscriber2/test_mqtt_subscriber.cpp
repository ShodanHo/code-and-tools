#include <iostream>
#include "mosquitto.h"
#include <mutex>

int run_subscriber(int argc, char* const argv[], void *data_ptr,
                   void (*message_callback)(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message));

#define HERE() __FUNCTION__ << ':'
#define STR(x) #x << '=' << x

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
  run_subscriber(argc, argv, &data, message_callback);
  return 0;
}
