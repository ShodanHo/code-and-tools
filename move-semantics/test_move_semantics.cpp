#include "a.h"
#include <iostream>

#define STR(x) #x << '=' << x
#define TOSTR(x) #x << '=' << x.toString()

A func();

int main()
{
  {
    A a({1,2,3,4,5});
    A b(a);
    A c(a);
    std::cout << TOSTR(a) << std::endl
	      << TOSTR(b) << std::endl
	      << TOSTR(c) << std::endl;
    std::cout << STR(&a) << std::endl
	      << STR(&b) << std::endl
	      << STR(&c) << std::endl;
  }
  {
    A d({1,2,3,4,5});
    std::cout << TOSTR(d) << std::endl;
    std::cout << STR(&d) << std::endl;
  }
  {
    A e = func();
    std::cout << TOSTR(e) << std::endl;
    std::cout << STR(&e) << std::endl;
  }
  return 0;
}
