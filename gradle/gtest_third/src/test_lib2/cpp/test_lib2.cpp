#include <iostream>

int main()
{
  std::cout << "Hello, world!\n";
#ifdef FLAVOUR1
  std::cout << "passing\n";
  return 0;
#endif
#ifdef FLAVOUR2
  std::cout << "failing\n";
  return 1;
#endif
}