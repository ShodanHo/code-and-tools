#include <iostream>
#include <vector>

#define STR(x) #x << '=' << x

int doubleFour(unsigned x)
{
  std::vector<unsigned> digits;
  while (x) {
    digits.push_back(x % 10);
    x /= 10;
  }
  std::size_t where = digits.size() + 1 - 4;
  while (digits.size()) {
    x *= 10;
    x += digits.back();
    if (digits.size() == where) {
      x *= 10;
      x += digits.back();
    }
    digits.pop_back();
  }
  return x;
}

int main()
{
  std::cout << STR(doubleFour(0)) << std::endl;
  std::cout << STR(doubleFour(1)) << std::endl;
  std::cout << STR(doubleFour(12)) << std::endl;
  std::cout << STR(doubleFour(123)) << std::endl;
  std::cout << STR(doubleFour(1234)) << std::endl;
  std::cout << STR(doubleFour(12345)) << std::endl;
  std::cout << STR(doubleFour(123456)) << std::endl;
  std::cout << STR(doubleFour(1234567)) << std::endl;
  std::cout << STR(doubleFour(356768)) << std::endl;
  return 0;
}
