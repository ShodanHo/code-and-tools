#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

#define STR(x) #x << '=' << x

template <class C>
std::ostream&
operator<<(std::ostream& os, const std::vector<C>& v)
{
  os << '{';
  for (typename std::vector<C>::const_iterator it = v.begin();
       it != v.end();
       ++it)
    {
      if (it != v.begin())
	os << ',';
      os << *it;
    }
  os << '}';
  return os;
}

/** \brief convert a time string to seconds.
 *
 * Convert a string in format mm:ss.sss to seconds.
 * eg 12:34.567 => 754.567
 * eg 34.567 => 34.567
 * \todo this can be made more bullet-proof
 */
double
string_to_double(const std::string &s)
{
  std::string str = s;
  const char *cp = strchr(str.c_str(), ':');
  if (cp == 0) {
    double seconds = atof(str.c_str());
    return seconds;
  }
  else {
    unsigned minutes = atoi(str.c_str());
    double seconds = atof(++cp);
    seconds += 60.0 * minutes;
    return seconds;
  }
}

int
main(int,char** argv)
{
  std::cout << argv[0] << " start" << std::endl;

  std::vector<std::string> strings;
  strings.push_back("00:00.000");
  strings.push_back("00:00.001");
  strings.push_back("10:00.000");

  std::cout << STR(strings) << std::endl;

  std::vector<double> doubles;
  doubles.resize(strings.size());

  std::transform(strings.begin(), strings.end(),
		 doubles.begin(), string_to_double);

  std::cout << STR(doubles) << std::endl;

  std::cout << argv[0] << " finish" << std::endl;
}
