#include <thread>
#include <sstream>
#include <popt.h>
#include <unistd.h> // sleep
#include "mqtt_publisher.h"
#include "mqtt_subscriber.h"
#include "common_utils.h"

enum {
  OPTION_HELP = 1,
  OPTION_BROKER,
  OPTION_PORT,
  OPTION_SUBSCRIBER_THREADS,
  OPTION_PUB_TOPIC,
  OPTION_SUB_TOPIC,
  OPTION_QOS,
};

static void
parse_command_line(int argc, char **argv, std::string& broker, int& port, unsigned& subThreads,
                   std::string& pubTopic, std::list<TopicQos>& topicQoses, int& qos)
{
  char *brokerPtr = nullptr;
  char *pubTopicPtr = nullptr;
  char *subTopicPtr = nullptr;

  struct poptOption options[] = {
      {"help", 'h', POPT_ARG_NONE, 0, OPTION_HELP, "print this message", 0 },
      {"broker", 'b', POPT_ARG_STRING, &brokerPtr, OPTION_BROKER, "broker name", "name" },
      {"port", 'p', POPT_ARG_INT, &port, OPTION_PORT, "set the MQTT port", "number" },
      {"subthreads", '\0', POPT_ARG_INT, &subThreads, OPTION_SUBSCRIBER_THREADS, "set number of threads", "number" },
      {"pubtopic", '\0', POPT_ARG_STRING, &pubTopicPtr, OPTION_PUB_TOPIC, "publisher topic", "topic" },
      {"subtopic", '\0', POPT_ARG_STRING, &subTopicPtr, OPTION_SUB_TOPIC, "subscriber topic", "topic" },
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
      case OPTION_SUBSCRIBER_THREADS:
        break;
      case OPTION_PUB_TOPIC:
        pubTopic = pubTopicPtr;
        break;
      case OPTION_SUB_TOPIC:
        topicQoses.push_back(TopicQos(subTopicPtr,qos));
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

struct SubscriberData
{
  std::mutex mutex;
  int count;
};

static void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
  SubscriberData* data_obj = reinterpret_cast<SubscriberData*>(obj);
  std::lock_guard<std::mutex> guard(data_obj->mutex);

  data_obj->count++;

  std::string payload(reinterpret_cast<char*>(message->payload),message->payloadlen);

  std::cout << HERE() << STR(message->topic) << ' '
      << STR(payload) << ' ' << STR(data_obj->count) << '\n';
}

int main(int argc, char **argv)
{
  std::string broker("localhost");
  int port = 1883;
  unsigned subThreads = 1;
  std::string pubTopic("hello/world");
  std::list<TopicQos> subTopicQoses;
  int qos = 0;
  parse_command_line(argc, argv, broker, port, subThreads, pubTopic, subTopicQoses, qos);

  conditioned_list<TopicPayloadQos> pubInput;

  pubInput.push_back(TopicPayloadQos(pubTopic,"hello",0));
  pubInput.push_back(TopicPayloadQos(pubTopic,"world",0));

  SubscriberData subscriberData; subscriberData.count = 0;

  mosquitto_lib_init();
  std::thread subscriber_thread(run_subscriber, &subscriberData, &broker, port, subThreads, &subTopicQoses, message_callback);
  std::thread publisher_thread(run_mqtt_publisher, &broker, 1883, &pubInput);

  int count = 0;
  for (;;) {
    std::ostringstream oss;
    oss << "count=" << count++;
    pubInput.push_back(TopicPayloadQos(pubTopic,oss.str(),0));
    if ((count % 10) == 0)
      sleep(2); // cause timeout
    else
      usleep(100000);
  }
  publisher_thread.join();
  return 0;
}
