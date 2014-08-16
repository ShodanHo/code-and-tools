#include <iostream>
#include <memory>
#include <vector>

using namespace std;

#define STR(x) #x << '=' << x
#define HERE() __FUNCTION__ << '(' << __LINE__ << "):"

class A
{
public:
    A(){cout << "constructor" << endl;};
    ~A(){cout << "destructor"  << endl;};
};

vector<shared_ptr<A> > global;

static void
test1()
{
  cout << HERE() << endl;

  shared_ptr<A> a (new A, [](A* p){delete p;}, std::allocator<A>());
  cout << HERE() << STR(a.use_count()) << endl;

  {
    shared_ptr<A> b (a);
    cout << HERE() << STR(a.use_count()) << endl;
    cout << HERE() << STR(b.use_count()) << endl;

    shared_ptr<A> c = a;
    cout << HERE() << STR(a.use_count()) << endl;

    global.push_back(a);
    cout << HERE() << STR(a.use_count()) << endl;
    global.push_back(a);
    cout << HERE() << STR(a.use_count()) << endl;
    global.push_back(a);
    cout << HERE() << STR(a.use_count()) << endl;
  }
  cout << HERE() << STR(a.use_count()) << endl;
}

static void
test2()
{
  A a;
  shared_ptr<A> sp(&a);

  global.push_back(sp);
} // this should terminate

int
main()
{
  test1();

  //test2(); // this terminates

  shared_ptr<A> a = global[0];

  cout << HERE() << STR(a.use_count()) << endl;

  {
    shared_ptr<A> x(a);

    cout << HERE() << STR(a.use_count()) << endl;

    {
      vector<shared_ptr<A> > vec;
      vec.push_back(x);

      cout << HERE() << STR(a.use_count()) << endl;
    }
    cout << HERE() << STR(a.use_count()) << endl;

    global.clear();

    cout << HERE() << STR(a.use_count()) << endl;
  }

  cout << HERE() << STR(a.use_count()) << endl;
  
  return 0;
}
