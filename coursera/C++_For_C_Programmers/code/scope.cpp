#include <iostream>

#define STR(x) #x << '=' << x

int main()
{
  int i = 9, j = 3;

  std::cout << STR(i) << ' ' << STR(j) << std::endl;
  {
    //int i = j + 2;
    int i(j+2);
    std::cout << STR(i) << ' ' << STR(j) << std::endl;
  }
  std::cout << STR(i) << ' ' << STR(j) << std::endl;

  return 0;
}
