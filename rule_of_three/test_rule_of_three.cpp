#include <iostream>

#define STR(x) #x << '=' << x

class test_private_destructor_t
{
  ~test_private_destructor_t();
public:
  int a;
  friend void delete_a(test_private_destructor_t*);
};

test_private_destructor_t::~test_private_destructor_t() {}

void delete_a(test_private_destructor_t* a)
{
  delete a;
}

class test_t
{
private:
  //test_t(const test_t&);
  //~test_t();

protected:
public:
  int a;
  //test_t();
  //test_t(const test_t&);
  //test_t& operator=(const test_t&);
};

//test_t::test_t() {}
//test_t::test_t(const test_t&) {}
//test_t& test_t::operator=(const test_t&) { return *this; }

std::ostream&
operator<<(std::ostream& os, const test_t& t)
{
  os << t.a;
  return os;
}

int
main()
{
  std::cout << "hello" << std::endl;

  test_t a; a.a = 1;
  test_t b(a);
  test_t c = a;

  test_private_destructor_t *d = new test_private_destructor_t;
  d->a = 1;
  delete_a(d);

  std::cout << STR(a) << std::endl;
  std::cout << STR(b) << std::endl;
  std::cout << STR(c) << std::endl;

  return 0;
}
