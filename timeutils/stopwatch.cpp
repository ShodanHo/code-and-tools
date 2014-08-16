#include "stopwatch.h"
#include "timeval_utils.h"
#include <sstream>

#define STR(x) #x << '=' << x

Stopwatch::Stopwatch() : start_time(), stop_time()
{
  gettimeofday(&start_time, 0);
  stop_time.tv_sec = stop_time.tv_usec = 0;
}

double Stopwatch::stop()
{
  gettimeofday(&stop_time, 0);
  return elapsed();
}

double Stopwatch::elapsed() const
{
  struct timeval tv = stop_time - start_time;
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

std::string Stopwatch::toString() const
{
  std::ostringstream oss;
  oss << STR(start_time) << "," << STR(stop_time);
  return oss.str();
}
