#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include <sys/time.h>
#include <string>

class Stopwatch
{
  struct timeval start_time;
  struct timeval stop_time;

public:
  Stopwatch();
  double stop();
  double elapsed() const;
  std::string toString() const;
};

#endif // __STOPWATCH_H__
