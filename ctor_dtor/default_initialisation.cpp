#include <iostream>

struct A {
  int a;
  A();
};

struct B : A {
  int b;
  B();
};

A::A():a(1) {}
B::B() {}

int main()
{
  {
    A a;
    std::cout << a.a << std::endl;
  }
  {
    B b;
    std::cout << b.a << std::endl;
    std::cout << b.b << std::endl;
  }
  return 0;
}
