#ifndef __TEST_HARNESS_H__
#define __TEST_HARNESS_H__

#include <iostream>
#include <list>
#include <stdio.h>

#define HERE() __FUNCTION__ << ':' << __LINE__
#define PASS() this->pass(__FUNCTION__,__LINE__)
#define FAIL() this->fail(__FUNCTION__,__LINE__)
#define NYI() this->notyetimplemented(__FUNCTION__,__LINE__)
#define FN() std::cout << HERE() << ":start" << std::endl
#define PASSFAIL(x) if (x) PASS(); else FAIL();
#define FAILPASS(x) if (x) FAIL(); else PASS();

/*! \brief Test harness framework.
 *
 * Test harness framework that can be sub-classed, and can keep track
 * of tests performed, tests passes, tests failed.
 */
class test_harness_t
{
  unsigned tests; //!< Detailed description after the member
  unsigned passes; /**< enum value 1 */

  /** \brief line numbers on which tests fail.
   */
  std::list<std::pair<unsigned,std::string> > fails;

  /** \brief line numbers on which not-yet-implemented occurs.
   */
  std::list<std::pair<unsigned,std::string> > nyi;

public:

  test_harness_t() : tests(0), passes(0), fails(), nyi() {}

  /**
   * called when a test passes.
   * @param str the function of the occurrence.
   * @param line the line number of the occurrence.
   */
  void pass(const char *str, unsigned line) {
    ++tests; ++passes;
    std::cout << "pass:" << str << ':' << line << std::endl;
  }

  /**
   * called when a test fails.
   * @param str the function of the occurrence.
   * @param line the line number of the occurrence.
   */
  void fail(const char *str, unsigned line) {
    ++tests; fails.push_back(std::pair<unsigned,std::string>(line,str));
    std::cout << "fail:" << str << ':' << line << std::endl;
  }

  /**
   * called when a test is not yet implemented.
   * @param str the function of the occurrence.
   * @param line the line number of the occurrence.
   */
  void notyetimplemented(const char *str, unsigned line) {
    ++tests; nyi.push_back(std::pair<unsigned,std::string>(line,str));
    std::cout << "nyi:" << str << ':' << line << std::endl;
  }

  /**
   * returns formatted results in one of two formats:
   *
   * "results total tests=  2 total passes=  2 total fails=  0"
   *
   * "results total tests=  4 total passes=  2 total fails=  2
   * failures={
   *   doSomeTests:98
   *   doSomeMoreTests:105
   * }"
   *
   * @return A string formatted with results
   */
  std::string results(void)
  {
    std::string ret("results ");

    {
      char resultsSummary[100];
      sprintf(resultsSummary,
	      "total tests=%3u total passes=%3u total fails=%3u total nyi=%3u",
	      tests, passes, fails.size(), nyi.size());
      ret += std::string(resultsSummary);
    }

    if (fails.size() > 0) {
      std::string summary("\nfailures={\n");

      std::list<std::pair<unsigned,std::string> >::const_iterator it;
      for (it = fails.begin(); it != fails.end(); ++it) {
	char str[50];
	sprintf(str,"  %s:%d\n", it->second.c_str(), it->first);
	summary += std::string(str);
      }

      ret += summary + std::string("}");
    }

    if (nyi.size() > 0) {
      std::string summary("\nnot yet implemented={\n");

      std::list<std::pair<unsigned,std::string> >::const_iterator it;
      for (it = nyi.begin(); it != nyi.end(); ++it) {
	char str[50];
	sprintf(str,"  %s:%d\n", it->second.c_str(), it->first);
	summary += std::string(str);
      }

      ret += summary + std::string("}");
    }
    return ret;
  }
};

#endif // __TEST_HARNESS_H__
