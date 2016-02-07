#include "readings.h"
#include <sstream> // ostringstream
#include "timeval_utils.h"

std::string readings_t::toString() const
{
  std::ostringstream oss;
  oss << '{' << mT << ',' << mVal << '}';
  return oss.str();
}

bool readings_t::operator==(const readings_t& reading) const
{
#define RETURN_FALSE_IF_NOT_EQUAL(a,member) { if (reading.member != this->member) return false; }
  RETURN_FALSE_IF_NOT_EQUAL(reading,mT);
  RETURN_FALSE_IF_NOT_EQUAL(reading,mVal);
  return true;
}

std::ostream& operator<<(std::ostream& os, const readings_t& r)
{
  os << r.toString();
  return os;
}

