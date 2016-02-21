#include <thread>
#include <sstream>
#include <popt.h>
#include <unistd.h> // sleep
#include "mqtt_publisher.h"

enum {
  OPTION_HELP = 1,
  OPTION_BROKER,
  OPTION_PORT,
  OPTION_TOPIC,
  OPTION_QOS,
};

static void
parse_command_line(int argc, char **argv, std::string& broker, int& port,
                   std::string& topic, int& qos)
{
  char *brokerPtr = nullptr;
  char *topicPtr = nullptr;

  struct poptOption options[] = {
      {"help", 'h', POPT_ARG_NONE, 0, OPTION_HELP, "print this message", 0 },
      {"broker", 'b', POPT_ARG_STRING, &brokerPtr, OPTION_BROKER, "broker name", "name" },
      {"port", 'p', POPT_ARG_INT, &port, OPTION_PORT, "set the MQTT port", "number" },
      {"topic", 't', POPT_ARG_STRING, &topicPtr, OPTION_TOPIC, "topic", "topic" },
      {"qos", 'q', POPT_ARG_INT, &qos, OPTION_QOS, "quality of service (0,1,2). Should precede topic", "number" },
      POPT_AUTOHELP
      { 0, 0, 0, 0, 0, 0, 0 }
  };

  poptContext context;
  context = poptGetContext(0, argc, (const char **)argv, options, 0);

  while (1)
  {
    int rc = poptGetNextOpt(context);

    if (rc > 0) {
      switch (rc) {
      case OPTION_HELP:
        poptPrintHelp(context, stdout, 0);
        exit(0);
      case OPTION_BROKER:
        broker = brokerPtr;
        break;
      case OPTION_PORT:
        break;
      case OPTION_TOPIC:
        topic = topicPtr;
        break;
      case OPTION_QOS:
        break;
      default:
        fprintf(stderr, "Unknown switch\n");
        exit(-1);
      }
    } else if (rc < -1) {
      fprintf(stderr, "Unknown parameter\n");
      exit(-1);
    } else {
      break;
    }
  }
}

int main(int argc, char **argv)
{
  std::string broker("localhost");
  int port = 1883;
  std::string topic("hello/world");
  int qos = 0;
  parse_command_line(argc, argv, broker, port, topic, qos);

  conditioned_list<TopicPayloadQos> input_list;

  input_list.push_back(TopicPayloadQos(topic,"hello",0));
  input_list.push_back(TopicPayloadQos(topic,"world",0));

  std::thread mqtt_publisher_thread(run_mqtt_publisher, "localhost", 1883, &input_list);

  int count = 0;
  for (;;) {
    std::ostringstream oss;
    oss << "count=" << count++;
    input_list.push_back(TopicPayloadQos(topic,oss.str(),0));
    if ((count % 10) == 0)
      sleep(2); // cause timeout
    else
      usleep(100000);
    if (count == 30) {
      input_list.push_back(TopicPayloadQos(topic,oss.str(),-1)); // quit
      break;
    }
  }
  mqtt_publisher_thread.join();
  return 0;
}
