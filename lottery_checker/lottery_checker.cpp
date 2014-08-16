#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#define STR(x) #x << '=' << x
#define HERE() __LINE__ << ':'
#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

template <class C>
std::ostream&
operator<<(std::ostream& os, const std::set<C>& s)
{
  os << '<';
  for (typename std::set<C>::const_iterator it = s.begin(); it != s.end(); ++it) {
    if (it != s.begin())
      os << ' '; // seperate elements other than first
    os << *it;
  }
  os << '>';
  return os;
}

#if 0
/**
 * ref: http://www.cplusplus.com/reference/cstdlib/qsort/
 */
static int
comp_unsigned(const void *a, const void *b)
{
  return (*(unsigned *)a - *(unsigned *)b);
}

/**
 * old-style C code for sorting.
 * quicker at run-time, but slower to understand, develop and maintain
 */
static size_t
NoDuplicates(unsigned *nums, size_t numNums)
{
  // do sort
  qsort(nums, numNums, sizeof(*nums), comp_unsigned);

  // look for adjacent duplicates
  while (true) {

    // do we have enough elements for a duplicate to exist
    if (numNums < 2)
      return numNums;

    // search for the first duplicate
    for (unsigned i = 0; i < numNums-1; ++i) {
      if (nums[i] == nums[i+1]) {
	// copy over the duplicate
	for (unsigned j = i; j < numNums-1; ++j)
	  nums[j] = nums[j+1];
	--numNums; // reduce the number of elements
	continue; // start again
      }
    }
    // if we get here, then no more duplicates exist
    return numNums;
  }
}
#endif

static std::string topMenu =
  "LITTLETON CITY LOTTO MODEL:\n"
  "---------------------------\n"
  "1) Play Lotto\n"
  "q) Quit Program\n"
  "Please make a selection:";

static std::string
getTopLevelMenuSelection()
{
  std::string selection;
  do {
    std::cout << topMenu;

    std::getline (std::cin, selection);
  } while ((selection != "1") && (selection != "q"));
  return selection;
}

static std::string
getUserName()
{
  std::string name;
  do {
    std::cout << "Please enter your name:";

    std::getline (std::cin, name);
  } while (name.size() == 0);
  return name;
}

static void
getLottoPicks(unsigned *picks, size_t numPicks)
{
  std::set<unsigned> userPicks;

  // get the required number of inputs
  do {
    std::cout << "Current picks are: " << userPicks << std::endl;
    std::cout << "Enter a pick (1-40):";

    std::string numberStr;
    std::getline (std::cin, numberStr);

    userPicks.insert(atoi(numberStr.c_str()));

    //std::cout << HERE() << STR(userPicks) << std::endl;

    // remove entries less than lower limit and greater than upper limit
    userPicks.erase(userPicks.begin(), userPicks.lower_bound(1));
    userPicks.erase(userPicks.upper_bound(40), userPicks.end());

    //std::cout << HERE() << STR(userPicks) << std::endl;

  } while (userPicks.size() < numPicks);

  // copy to the return parameter
  while (userPicks.size() > 0) {
    *picks++ = *userPicks.begin();
    userPicks.erase(userPicks.begin()); // remove first element
  }
}

static void
GenWinningNums(unsigned *nums, size_t numNums)
{
  /* if you are using C++ 2011 then this can be done better.
   * ref: http://www.cplusplus.com/reference/random/uniform_int_distribution/
   *
   * here we use the old method.
   * ref: http://www.cplusplus.com/reference/cstdlib/rand/?kw=rand
   */

  /* initialize random seed: */
  srand (time(NULL));

  std::set<unsigned> winningNums;

  // get the required number of numbers
  do {
    unsigned number = rand() % 50; // generate a random number between 0 and 49

    winningNums.insert(number);

    //std::cout << HERE() << STR(winningNums) << std::endl;

    winningNums.erase(winningNums.begin(), winningNums.lower_bound(1));
    winningNums.erase(winningNums.upper_bound(40), winningNums.end());

    //std::cout << HERE() << STR(winningNums) << std::endl;

  } while (winningNums.size() < numNums);

  // copy to the return parameter
  while (winningNums.size() > 0) {
    *nums++ = *winningNums.begin();
    winningNums.erase(winningNums.begin()); // remove first element
  }
}

/* count the overlap of two sets */
static unsigned
countSetOverlap(const std::set<unsigned>& a, const std::set<unsigned>& b)
{
  unsigned overlapCount = 0;
  for (std::set<unsigned>::const_iterator it = a.begin(); it != a.end(); ++it)
    if (b.count(*it) != 0) // should return 0 or 1
      ++overlapCount;
  return overlapCount;
}

static std::string
generateReport(const std::string& line1, // JULIE'S LOTTO RESULTS
	       const std::string& line3, // WINNING TICKET NUMBERS
	       const std::set<unsigned>& set1,
	       const std::string& line4, // JULIE'S TICKET
	       const std::set<unsigned>& set2,
	       const std::string& line5, // RESULTS:
	       const std::string& line6, // --------
	       const std::string& line7, // Number Matches: 
	       unsigned matches,
	       const std::string& line8) // Winnings      : SORRY NOTHING
{
  std::string ret;
  ret = line1 + '\n';
  ret += std::string(line1.size(), '-') + '\n';
  if (line3.size() >= line4.size()) {
    {
      std::stringbuf buffer;
      std::ostream os (&buffer);
      os << set1;
      std::string str1 = buffer.str().substr(1); // remove first '<'
      std::string str2 = str1.substr(0,str1.size()-1); // remove last '>'

      ret += line3 + ':' + str2 + '\n';
    }

    {
      std::stringbuf buffer;
      std::ostream os (&buffer);
      os << set2;
      std::string str1 = buffer.str().substr(1); // remove first '<'
      std::string str2 = str1.substr(0,str1.size()-1); // remove last '>'

      ret += line4 + std::string(line3.size() - line4.size(), ' ') + ':' + str2 + '\n';
    }
  } else {
    {
      std::stringbuf buffer;
      std::ostream os (&buffer);
      os << set1;
      std::string str1 = buffer.str().substr(1); // remove first '<'
      std::string str2 = str1.substr(0,str1.size()-1); // remove last '>'

      ret += line3 + std::string(line4.size() - line3.size(), ' ') + ':' + str2 + '\n';
    }

    {
      std::stringbuf buffer;
      std::ostream os (&buffer);
      os << set2;
      std::string str1 = buffer.str().substr(1); // remove first '<'
      std::string str2 = str1.substr(0,str1.size()-1); // remove last '>'

      ret += line4 + ':' + str2 + '\n';
    }
  }
  ret += line5 + '\n';
  ret += line6 + '\n';
  {
    char str[50];
    sprintf(str, "%u", matches);
    ret += line7 + str + '\n';
  }
  ret += line8;

  return ret;
}

int
main(int,char** argv)
{
  std::map<unsigned,std::string> win;
  win[7] = "JACKPOT - 1 MILLION";
  win[6] = "GREAT!- $100,000";
  win[5] = "LUCKY YOU! - $5,000";
  win[4] = "NOT BAD - $100";
  win[3] = "FREE TICKET";
  win[2] = win[1] = win[0] = "SORRY NOTHING";

  while (true) {
    std::string selection = getTopLevelMenuSelection();
    if (selection == "q")
      return 0;
    std::string userName = getUserName();

    //std::cout << STR(userName) << std::endl;

    unsigned UserTicket[7]; // requirement
    getLottoPicks(UserTicket, LENGTH(UserTicket));
    {
      std::set<unsigned> ticket(UserTicket, UserTicket + LENGTH(UserTicket));
      std::cout << STR(ticket) << std::endl;
    }

    unsigned WinningNums[7]; // requirement
    GenWinningNums(WinningNums, LENGTH(WinningNums));
    {
      std::set<unsigned> winning(WinningNums, WinningNums + LENGTH(WinningNums));
      std::cout << STR(winning) << std::endl;
    }

    unsigned winnings = 
      countSetOverlap(std::set<unsigned>(WinningNums, WinningNums + LENGTH(WinningNums)),
		      std::set<unsigned>(UserTicket, UserTicket + LENGTH(UserTicket)));

    std::cout << STR(winnings) << std::endl;

    if (win.find(winnings) == win.end()) { // should never occur
      std::cerr << "Error on line " << __LINE__ << std::endl;
      return -1;
    }

    std::string report =
      generateReport(userName + "'s LOTTO RESULTS",
		     std::string("WINNING TICKET NUMBERS"),
		     std::set<unsigned>(WinningNums, WinningNums + LENGTH(WinningNums)),
		     userName + "'s TICKET",
		     std::set<unsigned>(UserTicket, UserTicket + LENGTH(UserTicket)),
		     "RESULTS:",
		     "--------",
		     "Number Matches: ", winnings,
		     "Winnings      : " + win[winnings]);

    std::cout << report << std::endl;
  }

  return 0;
}
