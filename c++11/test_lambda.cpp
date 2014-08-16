#include <iostream>
#include <vector>

using namespace std;

#define STR(x) #x << '=' << x
#define HERE() __FUNCTION__ << '(' << __LINE__ << "):"

static void
func1()
{
  int i = 1;
  cout << [i](int j)->int{ return i + j; }(2) << endl;
  cout << [&i](int j)->int{ i = 2; return i + j; }(2) << endl;
  cout << [&](int j)->int{ i = 3; return i + j; }(2) << endl;
  cout << [=](int j)->int{ return i + j; }(2) << endl;
  cout << STR(i) << endl;
}

static void
func2()
{
  vector<int> v{1,2,3,4,5}; int sum = 0; for (auto i: v) [&](){ sum += i; }();
  cout << STR(sum) << endl;
}

int
main()
{
  cout << HERE() << endl;
  func1();
  func2();

  cout << HERE() << endl;
  
  return 0;
}
