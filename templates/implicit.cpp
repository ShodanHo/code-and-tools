#include "implicit.h"
#include <vector>
#include <list>
#include <map>

template < template<class,class> class C, class T>
std::ostream& operator<<(std::ostream& os, const C<T, std::allocator<T> >& c)
{
  os << '[';
  for (auto itr = c.cbegin(); itr != c.cend(); ++itr) {
    if (itr != c.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}

template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T,U>& p)
{
  os << '{' << p.first << ":" << p.second << '}';
  return os;
}

template < template<class,class,class,class> class C,
	   class Key, class T,
	   class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os, const C<Key, T, Compare, Alloc>& c)
{
  os << '[';
  for (auto itr = c.cbegin(); itr != c.cend(); ++itr) {
    if (itr != c.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}

// explicit instantiation
template std::ostream& operator<< <std::vector,int>(std::ostream& os, const std::vector<int, std::allocator<int> >& c);

template std::ostream& operator<< <std::list,int>(std::ostream& os, const std::list<int, std::allocator<int> >& c);

template
std::ostream& operator<< <std::map, int, std::string>(std::ostream& os, const std::map<int, std::string, std::less<int>, std::allocator<std::pair<int const, std::string> > >&);
