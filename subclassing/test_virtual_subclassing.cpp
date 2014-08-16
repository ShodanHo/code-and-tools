#include <iostream>
using namespace std;

#define HERE() __FUNCTION__ << ':' << __LINE__ << ':'

class A {
public:
  void Foo() {
    cout << HERE() << endl;
  }
};

class B : public virtual A {};
class C : public virtual A {};
class D : public B, public C {};

int main()
{
  D d;

  d.Foo();

  cout << "hello world" << endl;
  return 0;
}
