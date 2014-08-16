#include <iostream>
#include <math.h>

bool is_prime(int x)
{
  int count = 0;
  for (int i = 1; i < x; ++i) {
    if ((x % i) == 0) ++count;
  }
  if (count >= 2) return false;
  return true;
}

int main()
{
  while (1) {
    unsigned num;
    std::cin >> num;
    if (num == 0)
      break;
    if ((num >= 1) && (num <= 500)) {
      if (is_prime(num))
	std::cout << "TRUE" << std::endl;
      else
	std::cout << "FALSE" << std::endl;
    }
    else
      std::cout << "OUT OF RANGE" << std::endl;
  }
  return 0;
}
