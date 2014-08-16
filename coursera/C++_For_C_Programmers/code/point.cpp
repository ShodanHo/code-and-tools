#include <iostream>
#include <sstream>

#define STR(x) #x << '=' << x

template<typename T>
class point
{
  T x;
  T y;
public:
  point(const T& x, const T& y) : x(x), y(y) {}
  point(const point& p) : x(p.x), y(p.y) {}
  point operator=(const point& p) {
    if (p != *this) {
      x = p.x;
      y = p.y;
    }
  }

  point operator+(const point& p) { return point(x+p.x,y+p.y); }

  point& operator+=(const point& p) { x += p.x; y += p.y; return *this; }

  std::string toString() const;
};

template<typename T>
std::string point<T>::toString() const
{
  std::ostringstream oss;
  oss << '(' << x << ',' << y << ')';
  return oss.str();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const point<T>& p)
{
  os << p.toString();
  return os;
}

int main()
{
  point<double> p(3.5,2.5);
  std::cout << STR(p) << std::endl;
  point<double> q(2.5,4.5);
  std::cout << STR(q) << std::endl;
  std::cout << STR(p + q) << std::endl;
  p += q;
  std::cout << STR(p) << std::endl;

  return 0;
}
