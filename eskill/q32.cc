#include <iostream>
using namespace std;

int somevalue;

int main()
{
  somevalue = 42;
  int somevalue = 33;
  cout << ::somevalue << endl;
  cout << somevalue << endl;
  return 0;
}
