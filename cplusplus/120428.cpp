#include <iostream>
#include <limits>
using namespace std;

#define STR(x) #x << '=' << x

int main()
{
  std::cout << STR(numeric_limits<long>::max()) << endl
	    << STR(numeric_limits<unsigned long>::max()) << endl
	    << STR(numeric_limits<double>::max()) << endl
	    << STR(numeric_limits<unsigned long long>::max()) << endl;

  const int nLimit = 101;
  const int rLimit = nLimit;
  double C[nLimit][rLimit] = {};
  int count = 0;
  double max = 0;
	
  for	(int n = 0; n < nLimit; ++n)
    for	(int r = 0; r <= n; ++r)
    {
      if ((r == 0) || (r == n))
	C[n][r] = 1;
      else
	C[n][r] = C[n-1][r-1] + C[n-1][r];
      
      if (C[n][r] > max) {
	max = C[n][r];
      }
      if (C[n][r] < 0) {
	std::cout << STR(n) << ' ' << STR(r) << ' ' << STR(C[n][r]) << std::endl;
      }
      if (C[n][r] > 1000000) {
	std::cout << STR(n) << ' ' << STR(r) << ' ' << STR(C[n][r]) << std::endl;
	++count;
      }
    }

  std::cout << STR(max) << std::endl;
		
  cout << "Number of nCr greater than 1000000 is "
       << count << endl;
  return 0;
}
