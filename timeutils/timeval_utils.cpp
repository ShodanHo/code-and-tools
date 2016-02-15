#include "timeval_utils.h"
#include "stdio.h"
#include <sys/time.h>
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

bool operator!=(const struct timeval& tv1, const struct timeval& tv2)
{
  return !(tv1 == tv2);
}

bool operator<(const struct timeval& tv1, const struct timeval& tv2)
{
  if (tv1.tv_sec < tv2.tv_sec) return true;
  if ((tv1.tv_sec == tv2.tv_sec) && (tv1.tv_usec < tv2.tv_usec)) return true;
  return false;
}

bool operator<=(const struct timeval& tv1, const struct timeval& tv2)
{
  return !(tv2 < tv1);
}

bool operator>(const struct timeval& tv1, const struct timeval& tv2)
{
  return (tv2 < tv1);
}

bool operator>=(const struct timeval& tv1, const struct timeval& tv2)
{
  return (tv1 < tv2);
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

struct timeval operator*(uint32_t c, const struct timeval& tv)
{
  struct timeval ret = tv;
  uint64_t usec = static_cast<uint64_t>(ret.tv_usec) * c; // should not overflow for c < 1,000,000
  ret.tv_sec *= c;
  while (usec > 1000000) {
    ++ret.tv_sec;
    usec -= 1000000;
  }
  return ret;
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

struct timeval timeval_now(void)
{
  struct timeval now;
  gettimeofday(&now,0);
  return now;
}
