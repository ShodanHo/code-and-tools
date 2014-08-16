#include <iostream>
#include <complex>

using namespace std;

#define STR(x) #x << '=' << x

template<class T>
inline void myswap(T& a, T& b)
{
  T temp = a;
  a = b;
  b = temp;
}

int main()
{
  cout << endl;
  int m = 5;
  int n = 10;
  cout << STR(m) << ' ' << STR(n) << endl;
  myswap(m, n);
  cout << STR(m) << ' ' << STR(n) << endl;

  double x = 5.3;
  double y = 10.6;
  cout << STR(x) << ' ' << STR(y) << endl;
  myswap(x, y);
  cout << STR(x) << ' ' << STR(y) << endl;

  complex<double> r(2.4, 3.5);
  complex<double> s(3.4, 6.7);
  cout << STR(r) << ' ' << STR(s) << endl;
  myswap(r,s);
  cout << STR(r) << ' ' << STR(s) << endl;

  return 0;
}
