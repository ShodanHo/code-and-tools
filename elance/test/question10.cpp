#include <iostream>
#include <math.h>

bool is_cube(int x)
{
  double dcandidate = pow(10, log10(x) / 3.0);
  int candidate = dcandidate - 1;
  if ((candidate * candidate * candidate) == x)
    return true;
  candidate++;
  if ((candidate * candidate * candidate) == x)
    return true;
  candidate++;
  if ((candidate * candidate * candidate) == x)
    return true;
  return false;
}

int main()
{
  for (int i = 500; i <= 10000; ++i) {
    if (is_cube(i))
      std::cout << i << std::endl;
  }
  return 0;
}
