#include <thread>
#include "mutex_cond_collections.h"
#include <unistd.h> // sleep

int run_subscriber(int argc, char* const argv[]);

int main(int argc, char **argv)
{
#if 0
  conditioned_list<std::string> input_list;

  input_list.push_back(std::string("hello"));
  input_list.push_back(std::string("world"));
  input_list.push_back(std::string("hello world"));

  std::thread mqtt_publisher_thread(run_publisher, argc, argv, &input_list);

  int count = 0;
  for (;;) {
    char str[50];
    sprintf(str,"count=%d",count++);
    input_list.push_back(std::string(str));
    if ((count % 10) == 0)
      sleep(2); // cause timeout
    else
      usleep(100000);
  }
#else
  run_subscriber(argc, argv);
#endif
  return 0;
}
