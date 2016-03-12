#include <iostream>

#define GET(x) decltype(x) get##x(void) { return this->x; }

class test
{
  int a;
  char b;
public:
  test(decltype(a) _a, decltype(b) _b) : a(_a), b(_b) {}
  GET(a);
  GET(b);
};

int main(void)
{
  std::cout << "hello world\n";
  test t(1,2);
  std::cout << t.geta() << ' ' << (int)t.getb() << '\n';
  return 0;
}
