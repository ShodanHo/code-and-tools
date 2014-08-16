#include "a.h"
#include <iostream>

#define STR(x) #x << '=' << x

A func()
{
  std::cout << __FUNCTION__ << std::endl;
  A a({1,2,3});
  std::cout << STR(&a) << std::endl;
  std::cout << __FUNCTION__ << " return" << std::endl;
  return a;
}
