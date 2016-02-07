#ifndef __DETECTORS_H__
#define __DETECTORS_H__

#include <list>
#include "readings.h"

class detector_base_t
{
protected: // children can access
  std::list<readings_t> mData;
public:
  virtual ~detector_base_t() {}
  void add(std::list<readings_t>::const_iterator begin,
	   std::list<readings_t>::const_iterator end);
  virtual void process(std::list<readings_t>&) = 0;
  virtual std::string toString() const;

  friend class detector_test;
};

std::ostream& operator<<(std::ostream& os, const detector_base_t& d);

class positive_going_detector_t : public detector_base_t
{
public:
  virtual void process(std::list<readings_t>&);
};

#endif // __DETECTORS_H__
