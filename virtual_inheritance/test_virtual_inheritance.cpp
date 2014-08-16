#include <iostream>

class A {
public:
  int get() { return 0; }
};

class B: virtual public A {
};

class C: public virtual A {
};

class D: public B, public C {
};

int
main()
{
  std::cout << "hello world" << std::endl;
  D d;

  std::cout << static_cast<B>(d).get() << std::endl;
  std::cout << d.get() << std::endl;
  return 0;
}
