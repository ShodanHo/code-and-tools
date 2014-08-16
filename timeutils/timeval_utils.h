#ifndef __TIMEVAL_UTILS_H__
#define __TIMEVAL_UTILS_H__

#include "string"

std::string toString(const struct timeval& tv);
std::ostream& operator<<(std::ostream& os, const struct timeval& tv);
bool operator==(const struct timeval& tv1, const struct timeval& tv2);
struct timeval operator+(const struct timeval& tv1, const struct timeval& tv2);
struct timeval operator-(const struct timeval& tv1, const struct timeval& tv2);

#endif // __TIMEVAL_UTILS_H__
