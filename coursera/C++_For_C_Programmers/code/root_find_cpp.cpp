#include <iostream>
#include <cmath>

using namespace std;

/*
 * implement:
 * #define LENGTH(x) (sizeof(x)/sizeof(*(x)))
 * as a template
 *
 * ref:
 * http://stackoverflow.com/questions/3368883/
 * how-does-this-size-of-array-template-function-work
 */
template<typename T, std::size_t n> // compiler works these out
constexpr std::size_t array_size(T(&)[n]) // reference to an array of T
{
  return n; // return the elements in the array
}

// a function
static double fcn(double x)
{ return x * x - 1; }

// a function
static double fcn2(double x)
{ return x * x * x * x * x - exp(x) - 2; }

/*
 * structure of the data required to find the root of an equation
 * over a range.
 */
struct root_search_range_t {
  double (*function)(double); // single variable function
  double a; // lower bound
  double b; // upper bound
  double eps; // accuracy of root
};

static double root_find(const root_search_range_t& rsr)
{
  double (*function)(double) = rsr.function;
  double a = rsr.a;
  double b = rsr.b;
  double eps = rsr.eps;

  for (;;) {
    double midpoint = (b + a) / 2.0; // new attempt

    // is our interval within accuracy
    if (b - a < eps) {
      return midpoint;
    }

    // are we either side of the root
    if (function(a) * function(midpoint) <= 0) {
      b = midpoint; // yes
    } else {
      a = midpoint; // no
    }
  }
}

int main(void)
{
  cout << "Root finder" << endl;

  root_search_range_t searches[2] = { { fcn, 0.0, 4.0, 0.00001 },
				      { fcn2, 0.0, 14.0, 0.00001 } };

  for (unsigned i = 0; i < array_size(searches); ++i) {
    root_search_range_t fn = searches[i];

    double x = root_find(fn);

    cout << "root is at " << x << " with residual " << fn.function(x) << endl;
  }

  return 0;
}
