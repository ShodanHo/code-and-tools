#include <iostream>
#include <thread>
#include "timeval_utils.h"
#include <chrono>
#include <unistd.h>
#include "timed_wait.h"

#define HERE() __FUNCTION__ << '(' << __LINE__ << ')'
#define STR(x) #x << '=' << x

int main()
{
  conditioned_list<pulse_cmd_t> cmds;
  conditioned_list<int> state_output;
  std::thread timed_wait(do_timed_wait, &cmds, &state_output);

  sleep(1);
  pulse_cmd_t cmd(MA_PULSES_POS_NEG_START, // major_action_t,
                  timeval_ctor(10), // struct timeval pre,
                  timeval_ctor(5), // struct timeval positive,
                  timeval_ctor(10), // struct timeval period,
                  5); // unsigned count)

  cmds.push_back(cmd);
  for (auto quit = false; !quit;) {
    state_output.cond_wait();
    while (state_output.size() > 0) {
      auto x = state_output.front_value();
      state_output.pop_front();
      switch (x) {
        case 0: std::cout << x << " idle\n"; break;
        case 1: std::cout << x << " pre\n"; break;
        case 2: std::cout << x << " +ve\n"; break;
        case 3: std::cout << x << " -ve\n"; break;
        case 4: std::cout << x << " done\n"; quit = true; break;
      }
    }
  }
  std::cout << HERE() << " done\n";
  pulse_cmd_t term(MA_TERMINATE_THREAD, // major_action_t,
                   timeval_ctor(0), // struct timeval pre,
                   timeval_ctor(0), // struct timeval positive,
                   timeval_ctor(0), // struct timeval period,
                   0); // unsigned count)
  cmds.push_back(term);
  timed_wait.join();

  return 0;
}

