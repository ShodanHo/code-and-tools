#ifndef __TIMEVAL_UTILS_H__
#define __TIMEVAL_UTILS_H__

#include "string"

std::string toString(const struct timeval& tv);
std::ostream& operator<<(std::ostream& os, const struct timeval& tv);
bool operator==(const struct timeval& tv1, const struct timeval& tv2);
bool operator!=(const struct timeval& tv1, const struct timeval& tv2);
struct timeval operator+(const struct timeval& tv1, const struct timeval& tv2);
struct timeval operator-(const struct timeval& tv1, const struct timeval& tv2);
struct timeval timeval_ctor(void);
struct timeval timeval_ctor(decltype(timeval::tv_sec));
struct timeval timeval_ctor(decltype(timeval::tv_sec),decltype(timeval::tv_usec));
struct timeval timeval_now(void);

#endif // __TIMEVAL_UTILS_H__
