#include <iostream>

#define STR(x) #x << '=' << x

class A { int m_i; };
class B { double m_d; };
class C : public A, public B { char m_c; };
struct E : A, B { char m_d; };

int main()
{
  C d;
  A *b1 = &d;
  B *b2 = &d;

  E e;
  A *a1 = &e;

  const int a = (reinterpret_cast<char*>(b1) == reinterpret_cast<char*>(&d)) ? 1 : 2;
  const int b = (b2 == &d) ? 3 : 4;
  const int c = (reinterpret_cast<char*>(b1) == reinterpret_cast<char*>(b2)) ? 5 : 6;

  std::cout << a << b << c << std::endl;

  std::cout << STR(b1) << std::endl;
  std::cout << STR(b2) << std::endl;
  std::cout << STR(&d) << std::endl;
  std::cout << STR((void*)&d) << std::endl;

  std::cout << STR((b1 == &d)) << std::endl;
  std::cout << STR((reinterpret_cast<char*>(b1) == reinterpret_cast<char*>(&d))) << std::endl;

  std::cout << STR((b2 == &d)) << std::endl;
  std::cout << STR((reinterpret_cast<char*>(b2) == reinterpret_cast<char*>(&d))) << std::endl;

  return 0;
}
