#include <iostream>

int main()
{
  for (int i = 40; i <= 126; ++i) {
    std::cout << std::hex << i;
    std::cout << '-';
    std::cout << std::oct << i;
    std::cout << '-';
    std::cout << std::dec << static_cast<char>(i);
    std::cout << std::endl;
  }

  return 0;
}
