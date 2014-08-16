#include <iostream>

#define STR(x) #x << '=' << x

int a1;
float f1;
double d1;

int a2 = 1;
float f2;
double d2;

int main()
{
  std::cout << STR(a1) << std::endl;
  std::cout << STR(f1) << std::endl;
  std::cout << STR(d1) << std::endl;

  std::cout << STR(a2) << std::endl;
  std::cout << STR(f2) << std::endl;
  std::cout << STR(d2) << std::endl;

  std::cout << STR(&a1) << std::endl;
  std::cout << STR(&f1) << std::endl;
  std::cout << STR(&d1) << std::endl;

  std::cout << STR(&a2) << std::endl;
  std::cout << STR(&f2) << std::endl;
  std::cout << STR(&d2) << std::endl;
  return 0;
}
