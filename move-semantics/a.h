#ifndef __A_H__
#define __A_H__

#include <initializer_list>
#include <sstream>

struct A
{
  unsigned size;
  int *ptr;

  A() = delete;
  A(std::initializer_list<int>);
  A(const A&);
  ~A();

  A& operator=(const A&);

  std::string toString() const;
};

#endif // __A_H__
