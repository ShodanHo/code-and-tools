#include "timeval_utils.h"
#include "stdio.h"
#include <sstream>

std::string toString(const struct timeval& tv)
{
  char str[20];
  sprintf(str, "%06ld", tv.tv_usec);
  std::ostringstream oss;
  oss << tv.tv_sec << '.' << str;
  return oss.str();
}

std::ostream&
operator<<(std::ostream& os, const struct timeval& tv)
{
  os << toString(tv);
  return os;
}

bool operator==(const struct timeval& tv1, const struct timeval& tv2)
{
  return ((tv1.tv_sec == tv2.tv_sec) && (tv1.tv_usec == tv2.tv_usec));
}

struct timeval
operator+(const struct timeval& tv1, const struct timeval& tv2)
{
  struct timeval tv = { tv1.tv_sec + tv2.tv_sec, tv1.tv_usec + tv2.tv_usec };
  while (tv.tv_usec >= 1000000) {
    tv.tv_usec -= 1000000;
    tv.tv_sec += 1;
  }

  return tv;
}

struct timeval
operator-(const struct timeval& tv1, const struct timeval& tv2)
{
  struct timeval tv = { tv1.tv_sec - tv2.tv_sec, tv1.tv_usec - tv2.tv_usec };
  while (tv.tv_usec < 0) {
    tv.tv_usec += 1000000;
    tv.tv_sec -= 1;
  }

  return tv;
}

struct timeval timeval_ctor(void)
{
  struct timeval ret;
  ret.tv_sec = 0;
  ret.tv_usec = 0;
  return ret;
}

struct timeval timeval_ctor(decltype(timeval::tv_sec) sec)
{
  struct timeval ret;
  ret.tv_sec = sec;
  ret.tv_usec = 0;
  return ret;
}

struct timeval timeval_ctor(decltype(timeval::tv_sec) sec,
			    decltype(timeval::tv_usec) usec)
{
  struct timeval ret;
  ret.tv_sec = sec;
  ret.tv_usec = usec;
  return ret;
}

#include <sys/time.h>
struct timeval timeval_now(void)
{
  struct timeval now;
  gettimeofday(&now,0);
  return now;
}
