#include <iostream>

int main()
{
  for (unsigned i = 258; i <= 393; ++i)
    if ((i % 10) != 5)
      std::cout << i << std::endl;
  return 0;
}
