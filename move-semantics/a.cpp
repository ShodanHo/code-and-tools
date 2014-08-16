#include "a.h"
#include <iostream>

#define STR(x) #x << '=' << x

A::A(std::initializer_list<int> l) : size(l.size()), ptr(new int[size])
{
  std::cout << "A::" << __FUNCTION__ << '(' << __LINE__ << ')' << std::endl;
  int *p =ptr;
  for (auto it = l.begin(), end = l.end(); it != end; ++it)
    *p++ = *it;
}

A::A(const A& other) : size(other.size), ptr(new int[size])
{
  std::cout << "A::" << __FUNCTION__ << '(' << __LINE__ << ')' << std::endl;
}

A::~A()
{
  std::cout << "A::" << __FUNCTION__ << '(' << __LINE__ << ')' << std::endl;
  delete [] ptr;
}

std::string
A::toString() const
{
  std::ostringstream oss;
  oss << "(" << STR(size) << "," << STR(ptr) << ':';

  for (unsigned i = 0; i < size; ++i)
    if (i)
      oss << ',' << ptr[i];
    else
      oss << ptr[i];
  oss << ")";

  return oss.str();
}

