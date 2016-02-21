#include <mutex>
#include <thread>
#include <popt.h>
#include <unistd.h>
#include "mqtt_subscriber.h"
#include "common_utils.h"
#include "list_vector_stream.h"

enum {
  OPTION_HELP = 1,
  OPTION_BROKER,
  OPTION_PORT,
  OPTION_SUBSCRIBER_THREADS,
  OPTION_TOPIC,
  OPTION_QOS,
  OPTION_DEBUG_LEVEL_MAIN,
  OPTION_DEBUG_LEVEL_XYZ
};

static void
parse_command_line(int argc, char **argv, std::string& broker, int& port, unsigned& subThreads,
                   std::list<TopicQos>& topicQoses,
                   unsigned &mainDebugLevel, unsigned &xyzDebugLevel)
{
  char *brokerPtr = nullptr;
  char *topic = nullptr;
  int qos = 0;
  unsigned main_debug_level = 0;
  unsigned xyz_debug_level = 0;

  struct poptOption options[] = {
      {"help", 'h', POPT_ARG_NONE, 0, OPTION_HELP, "print this message", 0 },
      {"broker", 'b', POPT_ARG_STRING, &brokerPtr, OPTION_BROKER, "broker name", "name" },
      {"port", 'p', POPT_ARG_INT, &port, OPTION_PORT, "set the MQTT port", "number" },
      {"subthreads", '\0', POPT_ARG_INT, &subThreads, OPTION_SUBSCRIBER_THREADS, "set number of threads", "number" },
      {"topic", 't', POPT_ARG_STRING, &topic, OPTION_TOPIC, "topic", "topic" },
      {"qos", 'q', POPT_ARG_INT, &qos, OPTION_QOS, "quality of service (0,1,2). Should precede topic", "number" },
      {"debug", 'd', POPT_ARG_INT, &main_debug_level, OPTION_DEBUG_LEVEL_MAIN,
          "set the main debug level: 0(little) - 10(lots)", "main debug level" },
      {"dbgModuleXYZ", '\0', POPT_ARG_INT, &xyz_debug_level, OPTION_DEBUG_LEVEL_XYZ,
              "set the xyz debug level: 0(little) - 10(lots)", "xyz debug level" },
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
      case OPTION_TOPIC:
        topicQoses.push_back(TopicQos(topic,qos));
        break;
      case OPTION_QOS:
        break;
      case OPTION_DEBUG_LEVEL_MAIN:
        mainDebugLevel = main_debug_level;
        break;
      case OPTION_DEBUG_LEVEL_XYZ:
        xyzDebugLevel = xyz_debug_level;
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
  std::string broker("localhost");
  int port = 1883;
  unsigned subThreads = 1;
  std::list<TopicQos> topicQoses;
  unsigned mainDebugLevel = 0;
  unsigned xyzDebugLevel = 0;
  parse_command_line(argc, argv, broker, port, subThreads,
                     topicQoses, mainDebugLevel, xyzDebugLevel);

  std::cout << STR(broker) << ' '
      << STR(port) << ' '
      << STR(subThreads) << ' '
      << STR(topicQoses) << ' '
      << '\n';

  xyz data; data.count = 0;

  std::thread subscriber_thread(run_subscriber, &data, &broker, port, subThreads, &topicQoses, message_callback);

  for (;;)
    sleep(10);

  return 0;
}
