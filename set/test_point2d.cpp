#include <iostream>
#include <set>

#define STR(x) #x << '=' << x

typedef std::pair<double,double> Point2D;

std::ostream& operator<<(std::ostream& os, const Point2D& p)
{
  os << '(' << p.first << ',' << p.second << ')';
  return os;
}

template <class T, class U>
std::ostream& operator<<(std::ostream& os, const std::set<T,U>& s)
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

#if 0
bool fncomp (const Point2D& lhs, const Point2D& rhs) {
  if (lhs.first < rhs.first) return true;
  if (lhs.first > rhs.first) return false;
  if (lhs.second < rhs.second) return true;
  if (lhs.second > rhs.second) return false;
  return false;
}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

//bool operator<(const Point2D& lhs, const Point2D& rhs);
#endif

int
main()
{
  //bool(*fn_pt)(const Point2D&, const Point2D&) = fncomp;
  //std::set<Point2D,bool(*)(const Point2D&,const Point2D&)> s (fn_pt); // function pointer as Compare
  std::set<Point2D> s;

  s.insert(Point2D(0,0));
  s.insert(Point2D(1,0));
  s.insert(Point2D(1,1));
  s.insert(Point2D(0,1));
  s.insert(Point2D(1,0));

  std::cout << STR(s) << std::endl;

  return 0;
}
