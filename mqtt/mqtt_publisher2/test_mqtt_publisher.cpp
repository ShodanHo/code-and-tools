#include <thread>
#include "mutexed_collections.h"
#include <unistd.h> // sleep

int run_publisher(int argc, char* const argv[], mutexed_list<std::string> *input_list);

int main(int argc, char **argv)
{
  mutexed_list<std::string> input_list;

  input_list.push_back(std::string("hello"));
  input_list.push_back(std::string("world"));
  input_list.push_back(std::string("hello world"));

  std::thread x(run_publisher, argc, argv, &input_list);

  int count = 0;
  for (;;) {
    char str[50];
    sprintf(str,"count=%d",count++);
    input_list.push_back(std::string(str));
    usleep(100000);
  }
  return 0;
}
