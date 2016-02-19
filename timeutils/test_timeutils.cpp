#include <sstream>
#include "test_harness.h"
#include "timeval_utils.h"
#include "stopwatch.h"

#define STR(x) #x << '=' << x
#define TOSTR(x) #x << '=' << x.toString()

class timeutils_test : public test_harness_t
{
  void test_timeval_operators(void);
  void test_stopwatch(void);
  void test_streaming(void);

public:
  void runAllTests(void)
  {
    try {
      test_timeval_operators();
      test_stopwatch();
      test_streaming();
    } catch (...) {
      std::cout << __PRETTY_FUNCTION__
          << ":unhandled exception:" << std::endl;
    }
  }
};

void timeutils_test::test_timeval_operators(void)
{
  { // toString()
    struct timeval tv = { 1, 1 };
    std::string returned = toString(tv);
    std::string expected("1.000001");
    std::cout << HERE() << ":" << STR(returned) << std::endl
	      << HERE() << ":" << STR(expected) << std::endl;
    PASSFAIL(returned == expected);
  }

  { // operator==
    struct timeval tv1 = { 0, 0 };
    struct timeval tv2 = { 0, 1 };
    struct timeval tv3 = { 1, 0 };
    struct timeval tv4 = { 1, 1 };

    PASSFAIL(tv1 == tv1);
    PASSFAIL(tv2 == tv2);
    PASSFAIL(tv3 == tv3);
    PASSFAIL(tv4 == tv4);

    FAILPASS(tv1 == tv2);
    FAILPASS(tv1 == tv3);
    FAILPASS(tv1 == tv4);

    FAILPASS(tv2 == tv1);
    FAILPASS(tv2 == tv3);
    FAILPASS(tv2 == tv4);

    FAILPASS(tv3 == tv1);
    FAILPASS(tv3 == tv2);
    FAILPASS(tv3 == tv4);

    FAILPASS(tv4 == tv1);
    FAILPASS(tv4 == tv2);
    FAILPASS(tv4 == tv3);
  }
  { // operator+
    {
      struct timeval tv1 = { 1, 0 };
      struct timeval tv2 = { 0, 1 };
      struct timeval returned = tv1 + tv2;
      struct timeval expected = { 1, 1 };
      std::cout << HERE() << ":" << STR(returned) << std::endl
		<< HERE() << ":" << STR(expected) << std::endl;
      PASSFAIL(returned == expected);
    }
    {
      struct timeval tv1 = { 1, 999999 };
      struct timeval tv2 = { 0, 1 };
      struct timeval returned = tv1 + tv2;
      struct timeval expected = { 2, 0 };
      std::cout << HERE() << ":" << STR(returned) << std::endl
		<< HERE() << ":" << STR(expected) << std::endl;
      PASSFAIL(returned == expected);
    }
  }
  { // operator-
    struct timeval tv1 = { 1, 0 };
    struct timeval tv2 = { 0, 1 };
    struct timeval returned = tv1 - tv2;
    struct timeval expected = { 0, 999999 };
    std::cout << HERE() << ":" << STR(returned) << std::endl
	      << HERE() << ":" << STR(expected) << std::endl;
    PASSFAIL(returned == expected);
  }
}

void timeutils_test::test_stopwatch(void)
{
  Stopwatch sw;
  std::cout << TOSTR(sw) << std::endl;
  sw.stop();
  std::cout << TOSTR(sw) << std::endl
	    << STR(sw.elapsed()) << std::endl;
}

void timeutils_test::test_streaming(void)
{
  {
    std::string s("11.234567");

    std::istringstream iss(s);

    timeval tv;
    iss >> tv;
    std::cout << STR(tv) << '\n';

    PASSFAIL(tv < timeval_ctor(11,234568));
    PASSFAIL(tv > timeval_ctor(11,234550));
  }
  {
    std::string s("11.23456");

    std::istringstream iss(s);

    timeval tv;
    iss >> tv;
    std::cout << STR(tv) << '\n';
    PASSFAIL(tv == timeval_ctor(11,234560));
  }
  {
    std::string s("11.");

    std::istringstream iss(s);

    timeval tv;
    iss >> tv;
    std::cout << STR(tv) << '\n';
    PASSFAIL(tv == timeval_ctor(11));
  }
  {
    std::string s("11");

    std::istringstream iss(s);

    timeval tv;
    iss >> tv;
    std::cout << STR(tv) << '\n';
    PASSFAIL(tv == timeval_ctor(11));
  }
}

int main()
{
  timeutils_test test;
  test.runAllTests();

  std::cout << test.results() << std::endl;

  return 0;
}
