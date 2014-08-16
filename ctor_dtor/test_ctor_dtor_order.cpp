#include <iostream>

#define HERE() __FUNCTION__ << '(' << __LINE__ << "):"

struct A {
  A() { std::cout << "A()" << std::endl; }
  ~A() { std::cout << "~A()" << std::endl; }
};

struct B {
  B() { std::cout << "B()" << std::endl; }
  ~B() { std::cout << "~B()" << std::endl; }
};

class C {
  A a; // done first
  B b; // done second
public:
  C() : b(), a() /* writing in this order generates a warning:
		  * test_ctor_dtor_order.cpp: In constructor ‘C::C()’:
		  * test_ctor_dtor_order.cpp:17:5: warning: ‘C::b’ will be initialized after [-Wreorder]
		  * test_ctor_dtor_order.cpp:16:5: warning:   ‘A C::a’ [-Wreorder]
		  * test_ctor_dtor_order.cpp:19:3: warning:   when initialized here [-Wreorder]
		  */
  {}
};

class D : A, B {
public:
  D() { std::cout << "D()" << std::endl; }
};

int
main(int, char **argv)
{
  std::cout << argv[0] << " start" << std::endl;

  C c;

  std::cout << HERE() << std::endl;

  D d;

  return 0;
}
