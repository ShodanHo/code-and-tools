#include<iostream>
using namespace std;

#define STR(x) #x << '=' << x

int main()
{
  int b = 1000;
  int a = 25;

  //--- Debugged version of "enemy" code
  int &e = (a < b) ? a : b;

  cout << STR(a) << ' ' << STR(b) << endl;
  cout << "lower value is " << e << endl;

  for (int i=2; i<=e; i++) {
    cout << "trying " << i << endl;
    while ((a%i==0) && (b%i==0)) {
      cout << "no remainder for " << i << endl;
      a /= i;
      cout << "a changed to " << a <<endl;
      b /= i;
      cout << "b changed to " << b <<endl;
      cout << "lower value is " << e << endl;
    }
  }

  cout << "One: " << a << " / " << b << endl;

  return 0;
}
