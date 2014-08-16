#include <iostream>
#include <list>
#include <algorithm>
#include <stdio.h>

#define STR_OP(x) #x << '=' << x()

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

class collection_t
{
  std::list<int> mList;
public:
  collection_t() : mList() {}
  void push_back(const int &x) { mList.push_back(x); }
  void pop_front() { mList.pop_front(); }
  myiterator begin() { return myiterator(*mList.begin()); }
  myiterator end();

  std::string operator()() const;
};

std::string
collection_t::operator()() const
{
  std::string ret("{");
  std::list<int>::const_iterator it;
  for (it = mList.begin() ; it != mList.end(); )
    {
      char tmp[50];
      sprintf(tmp,"%d", *it);
      ret = ret + tmp;
      if (++it != mList.end())
	ret = ret + ',';
    }
  ret = ret + '}';
  return ret;
}

static void
addone(int& x)
{
  x++;
}

int
main(int, char **argv)
{
  std::cout << argv[0] << " start" << std::endl;

  collection_t collection;
  collection.push_back(1);
  collection.push_back(2);
  collection.push_back(3);
  collection.push_back(4);
  collection.push_back(5);
  collection.push_back(6);

  std::cout << STR_OP(collection) << std::endl;

  for_each(collection.begin(), collection.end(), addone);

  std::cout << argv[0] << " end" << std::endl;
  return 0;
}
