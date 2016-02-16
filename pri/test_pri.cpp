#include "test_harness.h"
#include <iostream>
#include "pri.h"

#define STR(x) #x << '=' << x

class pri_test : public test_harness_t
{
  void test_pos_neg(void);

public:
  void runAllTests(void)
  {
    try {
    	test_pos_neg();
    } catch (...) {
      std::cout << __PRETTY_FUNCTION__
                << ":unhandled exception:" << std::endl;
    }
  }
};

void
pri_test::test_pos_neg()
{
  PriParams params(timeval_ctor(5), // const struct timeval& delay_time,
                   timeval_ctor(5), // const struct timeval& positive_time,
                   timeval_ctor(10), // const struct timeval& cycle_time,
                   5); //unsigned cycle_number)
  PriState state;
  struct timeval now = timeval_ctor(0);
  PriOutputStatus outputState;

  outputState = runPri(PC_ABORT, params, state, now);
  PASSFAIL(outputState == PSS_NOT_STARTED);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(1));
  PASSFAIL(outputState == PSS_NOT_STARTED);

  outputState = runPri(PC_POS_NEG_START, params, state, now + timeval_ctor(2)); // start
  PASSFAIL(outputState == PSS_DELAY_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(3));
  PASSFAIL(outputState == PSS_DELAY_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(4));
  PASSFAIL(outputState == PSS_DELAY_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(5));
  PASSFAIL(outputState == PSS_DELAY_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(6));
  PASSFAIL(outputState == PSS_DELAY_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(7)); // +ve 1
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(8));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(9));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(10));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(11));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(12)); // -ve
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(13));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(14));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(15));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(16));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(17)); // +ve 2
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(18));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(19));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(20));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(21));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(22)); // -ve
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(23));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(24));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(25));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(26));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(27)); // +ve 3
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(28));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(29));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(30));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(31));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(32)); // -ve
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(33));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(34));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(35));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(36));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(37)); // +ve 4
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(38));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(39));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(40));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(41));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(42)); // -ve
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(43));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(44));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(45));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(46));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(47)); // +ve 5
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(48));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(49));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(50));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(51));
  PASSFAIL(outputState == PSS_POSITIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(52)); // -ve
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(53));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(54));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(55));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(56));
  PASSFAIL(outputState == PSS_NEGATIVE_PERIOD);

  outputState = runPri(PC_NOTHING, params, state, now + timeval_ctor(57));
  PASSFAIL(outputState == PSS_FINISHED);

}

int main()
{
  pri_test test;
  test.runAllTests();

  std::cout << test.results() << std::endl;

  return 0;
}

