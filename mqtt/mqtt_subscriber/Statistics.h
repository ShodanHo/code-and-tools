/******************************************************
FILE NAME: Statistics.h
DESCRIPTION:
Simple classes to do mean and standard deviation calculations.
*/

#ifndef STATISTICS_INCLUDED
#define STATISTICS_INCLUDED
#pragma once

#include <cstdint>
#include <cmath>

namespace common {
  namespace statistics {

    /** Collect statistics on a sample. */
    class SampleStatistics {
    public:
      SampleStatistics()
	{
	  count = 0;
	}

      void addSample(double x)
      {
	sum += x;
	sumSqr += x*x;
	count++;
	if (count == 1) {
	  min = x;
	  max = x;
	}
	else {
	  min = std::min(x, min);
	  max = std::max(x, max);
	}
      }

      void clear()
      {
	count = 0;
	sum = 0.0;
	sumSqr = 0.0;
      }
            
      bool isValid()
      {
	return (count > 0);
      }

      uint64_t getCount()
      {
	return count;
      }

      double getMean()
      {
	return (sum / count);
      }
            
      double getStandardDeviation()
      {
	double variance = (count*sumSqr - sum*sum) / (count*(count - 1));
	if (variance > 0) {
	  return sqrt(variance);
	}
	return 0.0;
      }
            
      double getMin()
      {
	return min;
      }
            
      double getMax()
      {
	return max;
      }

      double getSum()
      {
	return sum;
      }

    private:
      uint64_t count;
      double sum;
      double sumSqr;
      double min;
      double max;
    };


    /** Collect statistics on sample to sample differences. */
        class SampleDifferenceStatistics
        {
        public:
	SampleDifferenceStatistics() :
	  stats()
            {
	      ready = false;
            }

	  void addSample(double x)
	  {
	    if (ready) {
	      stats.addSample(x - lastSample);
	    }
	    lastSample = x;
	    ready = true;
	  }
	  void clear()
	  {
	    ready = false;
	    stats.clear();
	  }
	  bool isValid()
	  {
	    return (ready && stats.isValid());
	  }

	  uint64_t getCount()
	  {
	    return stats.getCount();
	  }

	  double getMean()
	  {
	    return stats.getMean();
	  }
	  double getStandardDeviation()
	  {
	    return stats.getStandardDeviation();
	  }
	  double getMin()
	  {
	    return stats.getMin();
	  }
	  double getMax()
	  {
	    return stats.getMax();
	  }
	  double getSum()
	  {
	    return stats.getSum();
	  }

        private:
	  SampleStatistics stats;
	  bool ready;
	  double lastSample;
        };

  }
}

#endif
