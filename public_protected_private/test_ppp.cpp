#include <iostream>
using namespace std;
#define HERE() __FUNCTION__<< '(' << __LINE__ << "):"
#define STR(x) #x << '=' << x

class A 
{
public:
  int x;
protected:
  int y;
private:
  int z;
};

class B : public A
{
  // x is public
  void X() { x = 1; }
  // y is protected
  void Y() { y = 1; }
  // z is not accessible from B
  void Z() { z = 1; } // error
};

class C : protected A
{
  // x is protected
  void X() { x = 1; }
  // y is protected
  void Y() { y = 1; }
  // z is not accessible from C
  void Z() { z = 1; } // error
};

class D : private A
{
  // x is private
  void X() { x = 1; }
  // y is private
  void Y() { y = 1; }
  // z is not accessible from D
  void Z() { z = 1; } // error
};

int main()
{
  std::cout << "hello" << std::endl;

  {
    A a;
    a.x = 1;
    a.y = 1; // error 
    a.z = 1; // error
  }

  {
    B b;
    b.x = 1;
    b.y = 1; // error 
    b.z = 1; // error
  }

  {
    C c;
    c.x = 1; // error
    c.y = 1; // error 
    c.z = 1; // error
  }

  {
    D d;
    d.x = 1; // error
    d.y = 1; // error 
    d.z = 1; // error
  }

  return 0;
}
