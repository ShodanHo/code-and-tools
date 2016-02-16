#include <iostream>
#include <chrono>         // std::chrono::seconds
#include "timed_wait.h"
#include <unistd.h>
#include <sstream>
#include <string>
#include "timeval_utils.h"

#define DEBUG

#define HERE() __FUNCTION__ << '(' << __LINE__ << ')'
#define STR(x) #x << '=' << x

typedef enum e_doing {
  eDOING_NOTHING,
  eDOING_PRE,
  eDOING_POSITIVE,
  eDOING_NEGATIVE
} PulseDoing;

std::string toString(const PulseDoing& pulseDoing)
{
#define CASE_LITERAL_STREAM_BREAK(x,y) case y: x << #y; break

  std::ostringstream oss;
  switch (pulseDoing) {
    CASE_LITERAL_STREAM_BREAK(oss,eDOING_NOTHING);
    CASE_LITERAL_STREAM_BREAK(oss,eDOING_PRE);
    CASE_LITERAL_STREAM_BREAK(oss,eDOING_POSITIVE);
    CASE_LITERAL_STREAM_BREAK(oss,eDOING_NEGATIVE);
  }
  return oss.str();
}

std::ostream& operator<<(std::ostream&os, const PulseDoing& pd)
{
  os << toString(pd);
  return os;
}

struct pulse_action_t
{
  timed_wait_action_t mAction;
  PulseDoing mDoing; // 0=nothing,1=prior,2=positive,3=negative
  struct timeval mStart;
  struct timeval mPre;
  struct timeval mPositive;
  struct timeval mPeriod;
  unsigned mPulseNum; // 0 .. mPulseCount-1
  unsigned mPulses;

  pulse_action_t(void)
  : mAction(MA_NOTHING)
  , mDoing(eDOING_NOTHING)
  , mStart(timeval_ctor(0))
  , mPre(timeval_ctor(0))
  , mPositive(timeval_ctor(0))
  , mPeriod(timeval_ctor(0))
  , mPulseNum(0)
  , mPulses(0) {}

  pulse_action_t(const timed_wait_action_t& action,
                 PulseDoing doing,
                 const struct timeval& start,
                 const struct timeval& pre,
                 const struct timeval& positive,
                 const struct timeval& period,
                 unsigned pulseNum,
                 unsigned pulses)
  : mAction(action)
  , mDoing(doing)
  , mStart(start)
  , mPre(pre)
  , mPositive(positive)
  , mPeriod(period)
  , mPulseNum(pulseNum)
  , mPulses(pulses) {}
  std::string toString(void) const;
};

std::string pulse_action_t::toString(void) const
{
  std::ostringstream oss;
  oss << STR(mDoing) << ','
      << STR(mStart) << ','
      << STR(mPre) << ','
      << STR(mPositive) << ','
      << STR(mPeriod) << ','
      << STR(mPulseNum) << ','
      << STR(mPulses);
  return oss.str();
}

std::ostream& operator<<(std::ostream&os, const pulse_action_t& pa)
{
  os << pa.toString();
  return os;
}

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

void do_timed_wait(conditioned_list<pulse_cmd_t> *cmds, conditioned_list<int> *state_output)
{
  pulse_action_t pulse_action;

  for (;;) {
    cmds->cond_wait_for(std::chrono::milliseconds(1));
    cmds->cond_wait_for(std::chrono::seconds(1));

    const timeval now = timeval_now(); // take the time

    if (cmds->size() > 0) {
      auto cmd = cmds->front_value();
      cmds->pop_front();
#ifdef DEBUG
      std::cout << STR(cmd) << '\n';
#endif

      switch(cmd.mAction) {
        case MA_NOTHING:
          pulse_action = pulse_action_t(MA_NOTHING, // mMajor_action
                                        eDOING_NOTHING, // mDoing(0)
                                        timeval_ctor(0), // mStart
                                        timeval_ctor(0), // mPre
                                        timeval_ctor(0), // mPositive
                                        timeval_ctor(0), // mPeriod
                                        0, // unsigned mPulseNum
                                        0); // unsigned mPulses;
#ifdef DEBUG
         std::cout << STR(pulse_action) << '\n';
#endif
          break;

        case MA_TERMINATE_THREAD:
          return;

        case MA_PULSES_POS_NEG_START:
          pulse_action = pulse_action_t(MA_PULSES_POS_NEG_START, // mAction
                                        eDOING_NOTHING, // mDoing(0)
                                        now, // mStart
                                        cmd.mPre, // mPre
                                        cmd.mPositive, // mPositive
                                        cmd.mPeriod, // mPeriod
                                        0, // unsigned mPulseNum
                                        cmd.mCount); // unsigned mPulses;
#ifdef DEBUG
          std::cout << "Initialised parameters\n";
          std::cout << STR(pulse_action) << '\n';
#endif
          break;

        case MA_PULSES_NEG_POS_START:
          break;
      }
    }

    // run "state machine"
    switch(pulse_action.mAction) {
      case MA_NOTHING:
        break;

      case MA_TERMINATE_THREAD: // shouldn't happen
        return;

      case MA_PULSES_POS_NEG_START:
        if (pulse_action.mPulseNum >= pulse_action.mPulses) {
#ifdef DEBUG
          std::cout << "done\n";
#endif
          state_output->push_back(0);
          pulse_action.mDoing = eDOING_NOTHING;
        }
        else
        {
#ifdef DEBUG
          struct timeval into = now - pulse_action.mStart;
#endif
          if (pulse_action.mDoing == eDOING_NOTHING)
          {
#ifdef DEBUG
            struct timeval left = now - pulse_action.mStart;
#endif
#ifdef DEBUG
            std::cout << "pre " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
            pulse_action.mDoing = eDOING_PRE;
            state_output->push_back(1);
          }
          else if (pulse_action.mDoing == eDOING_PRE)
          {
#ifdef DEBUG
            struct timeval left = now - pulse_action.mStart;
#endif
            if (now < (pulse_action.mStart + pulse_action.mPre)) {
#ifdef DEBUG
              std::cout << "pre " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
              pulse_action.mDoing = eDOING_PRE;
              //state_output->push_back(1);
            }
            else
            {
#ifdef DEBUG
              std::cout << "+ve " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
              pulse_action.mDoing = eDOING_POSITIVE;
              state_output->push_back(2);
            }
          }
          else if (pulse_action.mDoing == eDOING_POSITIVE)
          {
            struct timeval left = now - pulse_action.mStart - pulse_action.mPre - pulse_action.mPulseNum * pulse_action.mPeriod;
            if (left < pulse_action.mPositive)
            {
#ifdef DEBUG
              std::cout << "+ve " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
              pulse_action.mDoing = eDOING_POSITIVE;
              //state_output->push_back(2);
            }
            else
            {
#ifdef DEBUG
              std::cout << "-ve " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
              pulse_action.mDoing = eDOING_NEGATIVE;
              state_output->push_back(3);
            }
          }
          else
          {
            struct timeval left = now - pulse_action.mStart - pulse_action.mPre - pulse_action.mPulseNum * pulse_action.mPeriod;
            if (left < pulse_action.mPeriod)
            {
#ifdef DEBUG
              std::cout << "-ve " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
              pulse_action.mDoing = eDOING_NEGATIVE;
              //state_output->push_back(3);
            }
            else
            {
              if (++pulse_action.mPulseNum >= pulse_action.mPulses) {
#ifdef DEBUG
                std::cout << "done " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
                pulse_action.mDoing = eDOING_NOTHING;
                state_output->push_back(4); // signal finish
              }
              else
              {
#ifdef DEBUG
                std::cout << "+ve " << STR(into) << ' ' << STR(left) << ' ' << STR(now) << ' ' << STR(pulse_action) << '\n';
#endif
                pulse_action.mDoing = eDOING_POSITIVE;
                state_output->push_back(2);
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

