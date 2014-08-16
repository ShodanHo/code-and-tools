#include <iostream>

struct A {
  void func() { std::cout << __LINE__ << std::endl; }
};

struct B : A {
  void func(int) { std::cout << __LINE__ << std::endl; }
  //void func() const { std::cout << __LINE__ << std::endl; }
};


int main()
{
  A a;
  a.func();

  B b;
  b.func();

  return 0;
}
