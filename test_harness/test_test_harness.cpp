#include "test_harness.h"

class something_test : public test_harness_t
{
  void doSomeTests(void);
  void doSomeMoreTests(void);

public:
  void runAllTests(void)
  {
    doSomeTests();
    doSomeMoreTests();
  }
};

void
something_test::doSomeTests()
{
  PASS();
  FAIL();
  NYI();
}

void
something_test::doSomeMoreTests()
{
  PASS();
  FAIL();
  NYI();
}

int
main(int, char **argv)
{
  std::cout << argv[0] << " start" << std::endl;

  something_test test;
  test.runAllTests();

  std::cout << test.results() << std::endl;

  std::cout << argv[0] << " stop" << std::endl;
  return 0;
}
