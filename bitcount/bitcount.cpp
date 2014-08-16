#include <iostream>

unsigned
countBits(unsigned num)
{
  if (num == 0)
    return 0;

  if (num & 1)
    return 1 + countBits(num >> 1);

 return countBits(num >> 1);
}

int
main(int, char **argv)
{
  std::cout << countBits(0) << std::endl;
  std::cout << countBits(1) << std::endl;
  std::cout << countBits(2) << std::endl;
  std::cout << countBits(3) << std::endl;
  std::cout << countBits(4) << std::endl;
  std::cout << countBits(5) << std::endl;
  std::cout << countBits(-1) << std::endl;

  return 0;
}
