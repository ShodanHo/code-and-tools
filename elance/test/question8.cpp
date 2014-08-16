#include <iostream>
#include <set>

int main()
{
  std::set<std::string> str_set;

  for (int i = 0; i < 6; ++i) {
    std::string line;

    std::getline(std::cin,line);
    str_set.insert(line);
  }

  for (std::set<std::string>::const_iterator it = str_set.begin();
       it != str_set.end();
       ++it) {
    std::cout << *it << std::endl;
  }
  return 0;
}
