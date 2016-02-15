#include <iostream>
#include <chrono>         // std::chrono::seconds
#include "timed_wait.h"
#include <unistd.h>
#include <sstream>
#include <string>
#include "timeval_utils.h"

#define HERE() __FUNCTION__ << '(' << __LINE__ << ')'
#define STR(x) #x << '=' << x

typedef enum _e_minor_action
{
  MN_PULSE_DONE,
  MN_PULSE_PRE,
  MN_PULSE_POS,
  MN_PULSE_NEG,
} minor_action_t;

struct pulse_action_t
{
  major_action_t mMajor_action;
  minor_action_t mMinor_action;
  int mDoing; // 0=prior,1=positive,2=negative
  struct timeval mStart;
  struct timeval mPre;
  struct timeval mPositive;
  struct timeval mPeriod;
  unsigned mPulseNum; // 0 .. mPulseCount-1
  unsigned mPulses;

  pulse_action_t(void)
  : mMajor_action(MA_NOTHING)
  , mMinor_action(MN_PULSE_DONE)
  , mDoing(0)
  , mStart(timeval_ctor(0))
  , mPre(timeval_ctor(0))
  , mPositive(timeval_ctor(0))
  , mPeriod(timeval_ctor(0))
  , mPulseNum(0)
  , mPulses(0) {}

  pulse_action_t(const major_action_t& major_action,
                 const minor_action_t& minor_action,
                 int doing,
                 const struct timeval& start,
                 const struct timeval& pre,
                 const struct timeval& positive,
                 const struct timeval& period,
                 unsigned pulseNum,
                 unsigned pulses)
  : mMajor_action(major_action)
  , mMinor_action(minor_action)
  , mDoing(doing)
  , mStart(start)
  , mPre(pre)
  , mPositive(positive)
  , mPeriod(period)
  , mPulseNum(pulseNum)
  , mPulses(pulses) {}
};

std::string
pulse_cmd_t::toString(void) const
{
  std::ostringstream oss;
  oss << STR(mAction) << ','
      << STR(mPre) << ','
      << STR(mPositive) << ','
      << STR(mPeriod) << ','
      << STR(mCount);
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const pulse_cmd_t& pulse_cmd)
{
  os << pulse_cmd.toString();
  return os;
}

void do_timed_wait(conditioned_list<pulse_cmd_t> *cmds, conditioned_list<bool> *done_output)
{
  pulse_action_t pulse_action;

  for (;;) {
    cmds->cond_wait_for(std::chrono::milliseconds(1));

    timeval now = timeval_now(); // take the time

    if (cmds->size() > 0) {
      auto cmd = cmds->front_value();
      cmds->pop_front();
      std::cout << STR(cmd) << '\n';

      switch(cmd.mAction) {
        case MA_NOTHING:
          pulse_action = pulse_action_t(MA_NOTHING, // mMajor_action
                                        MN_PULSE_DONE, // mMinor_action
                                        0, // mDoing(0)
                                        timeval_ctor(0), // mStart
                                        timeval_ctor(0), // mPre
                                        timeval_ctor(0), // mPositive
                                        timeval_ctor(0), // mPeriod
                                        0, // unsigned mPulseNum
                                        0); // unsigned mPulses;
          break;

        case MA_TERMINATE_THREAD:
          return;

        case MA_PULSES_POS_NEG_START:
          pulse_action = pulse_action_t(MA_PULSES_POS_NEG_START, // mMajor_action
                                        MN_PULSE_PRE, // mMinor_action
                                        0, // mDoing(0)
                                        now, // mStart
                                        cmd.mPre, // mPre
                                        cmd.mPositive, // mPositive
                                        cmd.mPeriod, // mPeriod
                                        0, // unsigned mPulseNum
                                        cmd.mCount); // unsigned mPulses;
          std::cout << "Initialised parameters\n";
          break;

        case MA_PULSES_NEG_POS_START:
          break;
      }
    }

    // run "state machine"
    switch(pulse_action.mMajor_action) {
      case MA_NOTHING:
        break;

      case MA_TERMINATE_THREAD: // shouldn't happen
        return;

      case MA_PULSES_POS_NEG_START:
        if (pulse_action.mPulseNum >= pulse_action.mPulses) {
          std::cout << "done\n";
        }
        else
        {
          struct timeval into = now - pulse_action.mStart;
          if (pulse_action.mDoing == 0)
          {
            struct timeval left = now - pulse_action.mStart - pulse_action.mPre;
            if (now < (pulse_action.mStart + pulse_action.mPre)) {
              std::cout << "pre " << STR(into) << ' ' << STR(left) << '\n';
              pulse_action.mDoing = 0;
            }
            else
            {
              std::cout << "+ve " << STR(into) << ' ' << STR(left) << '\n';
              pulse_action.mDoing = 1;
            }
          }
          else if (pulse_action.mDoing == 1)
          {
            struct timeval left = now - pulse_action.mStart - pulse_action.mPre - pulse_action.mPulseNum * pulse_action.mPeriod;
            if (left < pulse_action.mPositive)
            {
              std::cout << "+ve " << STR(into) << ' ' << STR(left) << '\n';
              pulse_action.mDoing = 1;
            }
            else
            {
              std::cout << "-ve " << STR(into) << ' ' << STR(left) << '\n';
              pulse_action.mDoing = 2;
            }
          }
          else
          {
            struct timeval left = now - pulse_action.mStart - pulse_action.mPre - pulse_action.mPulseNum * pulse_action.mPeriod;
            if (left < pulse_action.mPeriod)
            {
              std::cout << "-ve " << STR(into) << ' ' << STR(left) << '\n';
              pulse_action.mDoing = 2;
            }
            else
            {
              if (++pulse_action.mPulseNum >= pulse_action.mPulses) {
                done_output->push_back(true); // signal finish
                std::cout << "done " << STR(into) << ' ' << STR(left) << '\n';
              }
              else
              {
                std::cout << "+ve " << STR(into) << ' ' << STR(left) << '\n';
                pulse_action.mDoing = 1;
              }
            }
          }
        }
        break;

      case MA_PULSES_NEG_POS_START:
        break;
    }
  }
}

#if 0
//void pause_thread(int n, conditioned_list<pulse_cmd_t> *cmds)
void pause_thread(conditioned_list<pulse_cmd_t> *cmds)
{
  int n = 5;
  for (;;) {
    //cmds.cond_wait();
    cmds->cond_wait_for(std::chrono::milliseconds(200));
    //std::this_thread::sleep_for (std::chrono::seconds(n));
    std::cout << "pause of " << n << " seconds ended\n";
  }
}
#endif
