#include <iostream>

int main()
{
  int a = 1;
  decltype(a) b = a; // do we have -std=c++11 in the compile
  std::cout << "Hello, world\n";
  std::cout << "b=" << b << '\n';
  return 0;
}

