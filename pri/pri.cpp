#include "pri.h"

PriOutputStatus
runPri(const PriCommand& cmd, const PriParams& priParams, PriState& priState, const struct timeval& when)
{
  if (cmd == PC_ABORT)
  {
    priState = PriState(PA_NOTHING, PSS_NOT_STARTED, when, 0);
    return PSS_NOT_STARTED;
  }

  if (priState.outputStatus == PSS_NOT_STARTED)
  {
    if (cmd == PC_POS_NEG_START)
    {
      priState = PriState(PA_PULSES_POS_NEG, PSS_DELAY_PERIOD, when, 0);
      return PSS_DELAY_PERIOD;
    }
    return PSS_NOT_STARTED;
  }

  if (priState.outputStatus == PSS_DELAY_PERIOD)
  {
    if (when < (priState.startTime + priParams.delayTime))
    {
      return PSS_DELAY_PERIOD;
    }
    else
    {
      priState = PriState(PA_PULSES_POS_NEG, PSS_POSITIVE_PERIOD, priState.startTime, 0);
      return PSS_POSITIVE_PERIOD;
    }
  }

  if (priState.outputStatus == PSS_POSITIVE_PERIOD)
  {
    if (when < (priState.startTime + priParams.delayTime +
        priState.cycleCount * priParams.cycleTime + priParams.positiveTime))
    {
      return PSS_POSITIVE_PERIOD;
    }
    else
    {
      priState = PriState(PA_PULSES_POS_NEG, PSS_NEGATIVE_PERIOD, priState.startTime, priState.cycleCount);
      return PSS_NEGATIVE_PERIOD;
    }
  }

  if (priState.outputStatus == PSS_NEGATIVE_PERIOD)
  {
    if (when < (priState.startTime + priParams.delayTime +
        (priState.cycleCount + 1) * priParams.cycleTime))
    {
      return PSS_NEGATIVE_PERIOD;
    }
    else
    {
      if ((priState.cycleCount + 1) < priParams.cycleNumber)
      {
        priState = PriState(PA_PULSES_POS_NEG, PSS_POSITIVE_PERIOD, priState.startTime, priState.cycleCount + 1);
        return PSS_POSITIVE_PERIOD;
      }
      else
      {
        priState = PriState(PA_PULSES_POS_NEG, PSS_FINISHED, priState.startTime, priState.cycleCount + 1);
        return PSS_FINISHED;
      }
    }
  }
  return PSS_FINISHED;
}
