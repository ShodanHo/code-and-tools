#include <iostream>
#include <set>

#define STR(x) #x << '=' << x

template <class T, class Compare>
std::ostream& operator<<(std::ostream& os, const std::multiset<T,Compare>& s)
{
  os << '[';
  for (auto itr = s.cbegin(); itr != s.cend(); ++itr) {
    if (itr != s.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}

bool fncomp (char lhs, char rhs) {
  if ((lhs == 'P') && (rhs != 'P')) return true;
  if ((lhs == 'S') && ((rhs == 'E') || (rhs == 'H'))) return true;
  if ((lhs == 'E') && (rhs == 'H')) return true;
  return false;
}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

int
main()
{
  bool(*fn_pt)(char,char) = fncomp;
  std::multiset<char,bool(*)(char,char)> s (fn_pt); // function pointer as Compare

  s.insert('S');
  s.insert('H');
  s.insert('E');
  s.insert('E');
  s.insert('P');

  std::cout << STR(s) << std::endl;

  return 0;
}
