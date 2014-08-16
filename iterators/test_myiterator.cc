#include <iostream>
#include <list>
#include <algorithm>

#define STR(x) #x << '=' << x
#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

std::ostream&
operator<<(std::ostream& os, const std::list<int>& intlist)
{
  os << '{';
  std::list<int>::const_iterator it;
  for (it = intlist.begin() ; it != intlist.end(); )
    {
      os << *it;
      if (++it != intlist.end())
	os << ',';
    }
  os << '}';
  return os;
}

// ref: http://www.cplusplus.com/reference/iterator/iterator/
class myiterator : public std::iterator<std::input_iterator_tag, int>
{
  int* p;
public:
  myiterator(int* x) :p(x) {}
  myiterator(const myiterator& mit) : p(mit.p) {}
  myiterator& operator++() {++p;return *this;}
  myiterator operator++(int) {myiterator tmp(*this); operator++(); return tmp;}
  bool operator==(const myiterator& rhs) {return p==rhs.p;}
  bool operator!=(const myiterator& rhs) {return p!=rhs.p;}
  int& operator*() {return *p;}
};

static void
addone(int& x)
{
  x++;
}

int
main(int, char **argv)
{
  std::cout << argv[0] << " start" << std::endl;

  int a[] = { 0, 1, 2, 3, 4 };
  std::list<int> x(a,  a + LENGTH(a));

  std::cout << STR(x) << std::endl;

  for_each(x.begin(), x.end(), addone);

  myiterator begin(a);
  myiterator end(a + 5);

  for_each(begin, end, addone);

  std::list<int> y(a,  a + LENGTH(a));

  std::cout << STR(x) << std::endl;
  std::cout << STR(y) << std::endl;
  //std::cout << STR(z) << std::endl;

  std::cout << argv[0] << " end" << std::endl;
  return 0;
}
