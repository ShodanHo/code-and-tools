#include "json_fmt.h"
#include "timeval_utils.h"

std::string JsonZTime::operator()() const
{
  return "\"time\":\"" + toString(tv) + "\"";
}

std::string JsonFmtValueRunningStopped::operator()() const
{
  return running ? "\"Value\":\"RUNNING\"" : "\"Value\":\"STOPPED\"";
}

std::string JsonFmtLabelXyzValueRunningZTime::operator()() const
{
  return std::string("{") + JsonKeyValue("label","xyz")() + "," +
      JsonFmtValueRunningStopped(running)() + "," + JsonZTime(tv)() + "}";
}

