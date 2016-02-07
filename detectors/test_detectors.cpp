#include "test_harness.h"
#include <sstream> // ostringstream
#include "timeval_utils.h"
#include "list_vector_stream.h"
#include "detectors.h"

#define STR(x) #x << '=' << x

class detector_test : public test_harness_t
{
  void test_positive_going_detector(void);
   
public:
  void runAllTests(void)
  {
    try {
      test_positive_going_detector();
    } catch (...) {
      std::cout << __PRETTY_FUNCTION__
                << ":unhandled exception:" << std::endl;
    }
  }
};

void detector_test::test_positive_going_detector(void)
{
  { // test empty container
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> events;
    detector.process(events);
    std::cout << STR(events) << '\n';
    std::cout << STR(detector) << '\n';
    PASSFAIL(events == std::list<readings_t>());
    PASSFAIL(detector.mData == std::list<readings_t>()); // empty

    std::list<readings_t> data;
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == std::list<readings_t>()); // empty
  }

  { // test adding data
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> data{{timeval_ctor(0,0),0},{timeval_ctor(0,1),1}};
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == data);
  }

  { // test multiple negative input
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> data{{timeval_ctor(0,0),0},{timeval_ctor(0,1),0}};
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == data);

    std::list<readings_t> events;
    detector.process(events);
    std::cout << STR(events) << '\n';
    std::cout << STR(detector) << '\n';
    PASSFAIL(events == std::list<readings_t>());
    PASSFAIL((detector.mData == std::list<readings_t>{{timeval_ctor(0,1),0}})); // has the last
  }

  { // test multiple positive input
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> data{{timeval_ctor(0,0),1},{timeval_ctor(0,1),1}};
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == data);

    std::list<readings_t> events;
    detector.process(events);
    std::cout << STR(events) << '\n';
    std::cout << STR(detector) << '\n';
    PASSFAIL(events == std::list<readings_t>());
    PASSFAIL((detector.mData == std::list<readings_t>{{timeval_ctor(0,1),1}})); // has the last
  }

  { // test single positive going input
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> data{{timeval_ctor(0,0),0},{timeval_ctor(0,1),1}};
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == data);

    std::list<readings_t> events;
    detector.process(events);
    std::cout << STR(events) << '\n';
    std::cout << STR(detector) << '\n';
    PASSFAIL((events == std::list<readings_t>{{timeval_ctor(0,1),1}}));
    PASSFAIL((detector.mData == std::list<readings_t>{{timeval_ctor(0,1),1}}));
  }

  { // test positive then negative going input
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> data{{timeval_ctor(0,0),0},{timeval_ctor(0,1),1},{timeval_ctor(0,2),0}};
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == data);

    std::list<readings_t> events;
    detector.process(events);
    std::cout << STR(events) << '\n';
    std::cout << STR(detector) << '\n';
    PASSFAIL((events == std::list<readings_t>{{timeval_ctor(0,1),1}}));
    PASSFAIL((detector.mData == std::list<readings_t>{{timeval_ctor(0,2),0}}));
  }

  { // test multiple positive then negative going input
    positive_going_detector_t detector;
    std::cout << STR(detector) << '\n';

    std::list<readings_t> data{
      {timeval_ctor(0,0),0},
      {timeval_ctor(0,1),1},
      {timeval_ctor(0,2),1},
      {timeval_ctor(0,3),0},
      {timeval_ctor(0,4),0},
      {timeval_ctor(0,5),1},
      {timeval_ctor(0,6),0},
      {timeval_ctor(0,7),1},
      {timeval_ctor(0,8),0},
      {timeval_ctor(0,9),1},
    };
    detector.add(data.begin(), data.end());
    std::cout << STR(detector) << '\n';
    PASSFAIL(detector.mData == data);

    std::list<readings_t> events;
    detector.process(events);
    std::cout << STR(events) << '\n';
    std::cout << STR(detector) << '\n';
    PASSFAIL((events == std::list<readings_t>{
      {timeval_ctor(0,1),1},
      {timeval_ctor(0,5),1},
      {timeval_ctor(0,7),1},
      {timeval_ctor(0,9),1}}));
    PASSFAIL((detector.mData == std::list<readings_t>{{timeval_ctor(0,9),1}}));
  }
}

int main()
{
  detector_test test;
  test.runAllTests();

  std::cout << test.results() << std::endl;

  return 0;
}
