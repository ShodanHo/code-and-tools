#ifndef __PRI_H__
#define __PRI_H__

#include "timeval_utils.h"

typedef enum _PriCommand
{
  PC_ABORT,
  PC_NOTHING, // continue
  PC_POS_NEG_START,
  PC_NEG_POS_START,
} PriCommand;

typedef enum _PriAction
{
  PA_NOTHING,
  PA_PULSES_POS_NEG, // first positive, last negative
  PA_PULSES_NEG_POS, // first negative, last positive
} PriAction;

typedef enum _PriOutputStatus
{
  PSS_NOT_STARTED,
  PSS_DELAY_PERIOD,
  PSS_POSITIVE_PERIOD,
  PSS_NEGATIVE_PERIOD,
  PSS_FINISHED
} PriOutputStatus;

struct PriParams
{
  struct timeval delayTime;
  struct timeval positiveTime;
  struct timeval cycleTime;
  unsigned cycleNumber;
  PriParams(const struct timeval& _delayTime,
            const struct timeval& _positiveTime,
            const struct timeval& _cycleTime,
            unsigned _cycleNumber)
  : delayTime(_delayTime)
  , positiveTime(_positiveTime)
  , cycleTime(_cycleTime)
  , cycleNumber(_cycleNumber) {}
};

struct PriState
{
  PriAction action;
  PriOutputStatus outputStatus;
  struct timeval startTime;
  unsigned cycleCount;
  PriState(PriAction _action,
           PriOutputStatus _outputStatus,
           const struct timeval& _startTime,
           unsigned _cycleCount)
  : action(_action)
  , outputStatus(_outputStatus)
  , startTime(_startTime)
  , cycleCount(_cycleCount) {}
  PriState()
  : action(PA_NOTHING)
  , outputStatus(PSS_NOT_STARTED)
  , startTime(timeval_ctor())
  , cycleCount(0) {}
};

PriOutputStatus runPri(const PriCommand& cmd, const PriParams& priParams, PriState& priState, const struct timeval& when);

#endif // __PRI_H__
