#ifndef __READINGS_H__
#define __READINGS_H__

#include <time.h> // timeval
#include <string> // string

struct readings_t
{
  timeval mT;
  int mVal;
  readings_t(const timeval t, int val) : mT(t), mVal(val) {}
  std::string toString() const;
  bool operator==(const readings_t&) const; // needed for debugging
};

std::ostream& operator<<(std::ostream& os, const readings_t& r);

#endif // __READINGS_H__
