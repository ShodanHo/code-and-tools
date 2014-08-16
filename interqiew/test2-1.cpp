#include <cstddef>
#include <iostream>

class A
{
public:
  A() : m_x(0) { }

public:
  static ptrdiff_t member_offset(const A &a)
  {
    const char *p = reinterpret_cast<const char*>(&a);
    const char *q = reinterpret_cast<const char*>(&a.m_x);

    return q - p;
  }

private:
  int m_x;
};

class B
  : public A
{
public:
  B() : m_x('a') { }

public:
  static int m_n;

public:
  static ptrdiff_t member_offset(const B &b)
  {
    const char *p = reinterpret_cast<const char*>(&b);
    const char *q = reinterpret_cast<const char*>(&b.m_x);

    return q - p;
  }

private:
  char m_x;
};

int B::m_n = 1;

class C
{
public:
  C() : m_x(0) { }
  virtual ~C() { }

public:
  static ptrdiff_t member_offset(const C &c)
  {
    const char *p = reinterpret_cast<const char*>(&c);
    const char *q = reinterpret_cast<const char*>(&c.m_x);
    
    return q - p;
  }

private:
  int m_x;
};

int main()
{
  A a;
  B b;
  C c;
  std::cout << ((A::member_offset(a) == 0) ? 0 : 1);
  std::cout << ((B::member_offset(b) == 0) ? 0 : 2);
  std::cout << ((A::member_offset(b) == 0) ? 0 : 3);
  std::cout << ((C::member_offset(c) == 0) ? 0 : 4);
  std::cout << std::endl;

  return 0;
}
