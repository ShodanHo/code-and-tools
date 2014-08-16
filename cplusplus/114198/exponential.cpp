#include <iostream>     // std::cout
#include <iomanip>      // std::setprecision
#include <math.h>       // log10
using namespace std;

int main()
{
  double x;
  cout << "       x:"; cin >> x;

  double accuracy;
  cout << "accuracy:"; cin >> accuracy;

  double term = 1; // n = 0;
  double result = term; // 1 + ...

  for (unsigned n = 1; 2*term > accuracy; ++n) {
    term = term * x / n; // the n-th term
    result += term; // ... + x^n/n!
  }

  cout << "  exp(x):"
       << std::setprecision(accuracy < 1 ? -log10(accuracy) + 2 : 3)
       << result << endl;

  return 0;
}
