#include <iostream>
#include <stdlib.h>
#include <set>

int main()
{
  std::string count1_str;
  getline(std::cin, count1_str); 
  int count1 = atoi(count1_str.c_str());

  std::string count2_str;
  getline(std::cin, count2_str); 
  int count2 = atoi(count2_str.c_str());

  std::set<int> int_set;

  for (int i = 0; i < count1; ++i) {
    int x;
    std::cin >> x;
    int_set.insert(x);
  }

  for (int i = 0; i < count2; ++i) {
    int x;
    std::cin >> x;
    int_set.insert(x);
  }

  for (std::set<int>::const_reverse_iterator rit = int_set.rbegin();
       rit != int_set.rend();
       ++rit) {
    std::cout << *rit << std::endl;
  }

  return 0;
}
