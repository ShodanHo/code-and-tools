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
  conditioned_list<bool> done;
  std::thread timed_wait(do_timed_wait, &cmds, &done);

  sleep(1);
  pulse_cmd_t cmd(MA_PULSES_POS_NEG_START, // major_action_t,
                  timeval_ctor(10), // struct timeval pre,
                  timeval_ctor(5), // struct timeval positive,
                  timeval_ctor(10), // struct timeval period,
                  5); // unsigned count)

  cmds.push_back(cmd);
  done.cond_wait();
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

