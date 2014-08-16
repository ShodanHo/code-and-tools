#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <map>

using namespace std;

#define STR(x) #x << '=' << x

#if 1
template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s)
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
#else
//template < class T,                        // set::key_type/value_type
//           class Compare = less<T>,        // set::key_compare/value_compare
//           class Alloc = allocator<T> >    // set::allocator_type
//           > class set;
template < template<class,class,class> class C, class T, class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os, const C<T, Compare, Alloc>& s)
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
#endif

#if 0
template <class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
  os << '[';
  for (auto itr = v.cbegin(); itr != v.cend(); ++itr) {
    if (itr != v.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}
#endif

//template < class T, class Alloc = allocator<T> > class list;
//template < class T, class Alloc = allocator<T> > class vector;
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

#if 0
template <class T, class U>
ostream& operator<<(ostream& os, const map<T,U>& v)
{
  os << '[';
  for (auto itr = v.cbegin(); itr != v.cend(); ++itr) {
    if (itr != v.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}
#endif

/*
template < class Key,                                     // map::key_type
           class T,                                       // map::mapped_type
           class Compare = less<Key>,                     // map::key_compare
           class Alloc = allocator<pair<const Key,T> >    // map::allocator_type
           > class map;
*/
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

int main()
{
  {
    set<int> set;
    cout << STR(set) << endl;
  }
  {
    set<int> set;
    set.insert(3);
    set.insert(2);
    set.insert(1);
    set.insert(0);
    cout << STR(set) << endl;
  }
  {
    vector<int> vector;
    cout << STR(vector) << endl;
  }
  {
    vector<int> vector(10);
    for (auto& v: vector) {
      static int i = 0;
      v = i++;
    }

    cout << STR(vector) << endl;
  }

  {
    list<int> list;
    cout << STR(list) << endl;
  }
  {
    list<int> list(10);
    for (auto& l: list) {
      static int i = 0;
      l = i++;
    }

    cout << STR(list) << endl;
  }

  {
    map<int,string> map;
    cout << STR(map) << endl;
  }
  {
    map<int,string> map;
    string str[] = { "zero", "one", "two", "three", "four",
		     "five", "six", "seven", "eight", "nine" };
    for (auto& s: str) {
      static int i = 0;
      map[i] = s;
      ++i;
    }

    cout << STR(map) << endl;
  }

  return 0;
}
