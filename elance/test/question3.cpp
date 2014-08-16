#include <iostream>
#include <set>
#include <ctype.h>
using namespace std;

int main()
{
  std::set<std::string> pallin;
  for (int i = 0; i < 4; ++i) {
    std::string line;

    std::getline(std::cin, line);
    std::string reversed = line;

    std::string::iterator it;
    std::string::reverse_iterator rit;
    for (it = line.begin(), rit = reversed.rbegin();
	 it != line.end();
	 ++it, ++rit) {
      *rit = *it = tolower(*it);
    }

    if (line == reversed)
      pallin.insert(line);
  }

  for (std::set<std::string>::const_iterator it = pallin.begin();
       it != pallin.end();
       ++it)
    std::cout << *it << std::endl;
  return 0;
}
