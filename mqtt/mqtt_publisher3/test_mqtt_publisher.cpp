#include <thread>
#include <sstream>
#include <unistd.h> // sleep
#include "mqtt_publisher.h"

int main(int argc, char **argv)
{
  conditioned_list<TopicPayloadQos> input_list;

  input_list.push_back(TopicPayloadQos("temperature/celsius","hello",0));
  input_list.push_back(TopicPayloadQos("temperature/celsius","world",0));

  std::thread mqtt_publisher_thread(run_mqtt_publisher, "localhost", 1883, &input_list);

  int count = 0;
  for (;;) {
    std::ostringstream oss;
    oss << "count=" << count++;
    input_list.push_back(TopicPayloadQos("temperature/celsius",oss.str(),0));
    if ((count % 10) == 0)
      sleep(2); // cause timeout
    else
      usleep(100000);
    if (count == 30) {
      input_list.push_back(TopicPayloadQos("temperature/celsius",oss.str(),-1)); // quit
      break;
    }
  }
  mqtt_publisher_thread.join();
  return 0;
}
