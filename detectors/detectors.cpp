#include "detectors.h"
#include <sstream> // std::ostringstream
#include "list_vector_stream.h"
#include <algorithm> // find_if

#define STR(x) #x << '=' << x

void detector_base_t::add(std::list<readings_t>::const_iterator begin,
			  std::list<readings_t>::const_iterator end)
{
  std::copy (begin,end,back_inserter(mData));
}

std::string
detector_base_t::toString() const
{
  std::ostringstream oss;
  oss << '{' << STR(mData) << '}';
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const detector_base_t& d)
{
  os << d.toString();
  return os;
}

void
positive_going_detector_t::process(std::list<readings_t>& output)
{
  while (mData.size() > 1)
  {
    if (mData.front().mVal == 0)
    {
      std::list<readings_t>::const_iterator
	it = std::find_if(mData.begin(), mData.end(),
			  [](const readings_t& r) { return r.mVal; } ); // find the first 1
      if (it != mData.end()) // have we found a transition
      {
	output.push_back(*it);
	mData.erase(mData.begin(),it); // erase up to the last
      }
      else
      {
	mData.erase(mData.begin(),--mData.end()); // erase up to the last
      }	
    }
    else
    {
      std::list<readings_t>::const_iterator
	it = std::find_if(mData.begin(), mData.end(),
		   [](const readings_t& r) { return !r.mVal; } ); // find the first 0
      if (it != mData.end()) // have we found a transition
      {
	mData.erase(mData.begin(),it);
      }
      else
      {
	mData.erase(mData.begin(),--mData.end()); // erase up to the last
      }	
    }
  }
}

