#include <iostream>

#define STR(x) #x << '=' << x

//template <class T, T* initial>
template <class T>
T sum(const T data[], std::size_t size, T s = T(0))
{
  for (std::size_t i = 0; i < size; ++i) {
    s += data[i];
  }
  return s;
}

template <typename T, std::size_t Length>
constexpr std::size_t array_length(T(&)[Length])
{
  return Length;
}

int
main()
{
  std::cout << "hello" << std::endl;
  {
    int x[] = { 1, 2, 3, 4 };
    int accum = sum(x, array_length(x));
    std::cout << STR(accum) << std::endl;
    accum = sum(x, array_length(x), accum);
    std::cout << STR(accum) << std::endl;
  }
  {
    double x[] = { 1.1, 2.1, 3.1, 4.1 };
    double accum = sum(x, array_length(x));
    std::cout << STR(accum) << std::endl;
    accum = sum(x, array_length(x), accum);
    std::cout << STR(accum) << std::endl;
  }
  return 0;
}
