#include <iostream>

#define STR(x) #x << '=' << x

struct A {
private:
  int p;
public:

  friend int main();
};

struct B {
protected:
  int p;
public:

  friend int main();
};

struct AA : A {
private:
  int q;
};
struct BB : B {
};

struct C {
protected:
  int p;
};

struct CC : C {
  friend int main();
};

int main()
{
  {
    A a;
    a.p = 1; // ok, we're friends
    std::cout << STR(a.p) << std::endl;
  }
  {
    B b;
    b.p = 1; // ok, we're friends
    std::cout << STR(b.p) << std::endl;
  }
  {
    AA aa;
    aa.p = 1; // ok, we're friends
    std::cout << STR(aa.p) << std::endl;
  }
  {
    //AA aa;
    //aa.q = 1; // error, friend stays in struct/class
    //std::cout << STR(aa.q) << std::endl;
  }
  {
    BB bb;
    bb.p = 1; // ok, we're friends
    std::cout << STR(bb.p) << std::endl;
  }
  {
    CC cc;
    cc.p = 1;
    std::cout << STR(cc.p) << std::endl;
  }
  return 0;
}
