#ifndef __TIMED_WAIT_H__
#define __TIMED_WAIT_H__

#include "mutex_cond_collections.h"
#include <time.h>

typedef enum _e_timed_wait_action
{
  MA_NOTHING,
  MA_TERMINATE_THREAD,
  MA_PULSES_POS_NEG_START, // first positive, last negative
  MA_PULSES_NEG_POS_START, // first negative, last positive
} timed_wait_action_t;

struct pulse_cmd_t
{
  timed_wait_action_t mAction;
  struct timeval mPre;
  struct timeval mPositive;
  struct timeval mPeriod;
  unsigned mCount;
  pulse_cmd_t(timed_wait_action_t action,
              struct timeval pre,
              struct timeval positive,
              struct timeval period,
              unsigned count)
  : mAction(action)
  , mPre(pre)
  , mPositive(positive)
  , mPeriod(period)
  , mCount(count) {}
  std::string toString(void) const;
};

std::ostream& operator<<(std::ostream& os, const pulse_cmd_t& pulse_cmd);

void do_timed_wait(conditioned_list<pulse_cmd_t> *cmds, conditioned_list<bool> *done_output);

#endif // __TIMED_WAIT_H__
