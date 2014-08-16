#include <iostream>

#define STR(x) #x << '=' << x

typedef enum days { SUN, MON, TUE, WED, THU, FRI, SAT };

days operator+(days d)
{
  return static_cast<days>( (static_cast<unsigned>(d) + 1) % 7);
}

days operator-(days d)
{
  return static_cast<days>( (static_cast<unsigned>(d) + 6) % 7);
}

std::ostream&
operator<<(std::ostream& os, const days& day)
{
  switch (day) {
  case SUN: os << "SUN"; break;
  case MON: os << "MON"; break;
  case TUE: os << "TUE"; break;
  case WED: os << "WED"; break;
  case THU: os << "THU"; break;
  case FRI: os << "FRI"; break;
  case SAT: os << "SAT"; break;
  default : os << "***"; break;
  }
  return os;
}

int main()
{
  days d = SUN, e;
  e = +d;

  std::cout << STR(e) << std::endl;

  std::cout << STR(d) << std::endl;

  std::cout << STR(+d) << std::endl;

  std::cout << STR(d) << std::endl;

  std::cout << STR(-d) << std::endl;

  std::cout << STR(d) << std::endl;

  return 0;
}
