#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;
	
int find_pos(const std::string& str, const std::string & what)
{
  std::size_t pos = str.find(what);

  if (pos == string::npos)
    return -1;
  return pos;
}
	
int main()
{
  unsigned count;

  std::string str;
  std::getline(std::cin,str);
  count = atoi(str.c_str());

  std::vector<std::string> lines;
  while (lines.size() < count) {
    std::string line;
    std::getline(std::cin, line);
    lines.push_back(line);
  }

  for (std::vector<std::string>::const_iterator it
	= lines.begin();
       it != lines.end();
       ++it) {
    std::cout << find_pos(*it, "is") + find_pos(*it, "a") << std::endl;
  }
  return 0;
}
