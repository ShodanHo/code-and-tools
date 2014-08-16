#include <iostream>

class B;

class A {
  int _i;
public:
  A(int i) : _i(i) {}
  operator B(void) const;
};

class B {
  int _i;
public:
  B(int i) : _i(i) {}
  B(const A&);
};

A::operator B(void) const
{
  return B(_i);
}

B::B(const A&)
{
}

int
main()
{
  std::cout << "Hello world" << std::endl;

  A a(0);
  B b(1);

  B d(a);

  return 0;
}
