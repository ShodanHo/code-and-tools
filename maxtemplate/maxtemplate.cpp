#include <iostream>

template<class TYPE>
TYPE MaxValue(const TYPE values[], unsigned length)
{
  TYPE Max=values[0];
  
  for (unsigned i = 1; i < length; ++i)
  {
    if (values[i] > Max)
    {
      Max = values[i];
    }
  }
  return Max;
}

using namespace std;

int main()
{
  int integers[] = {4, 25, 32, 85, 150, 12, 98, 200};
  cout << "Biggest integer: " << MaxValue<int>(integers, 8) << endl;

  double dvalues[] = { -1.0, 0.0, 1.2 };
  cout << "Biggest double: " << MaxValue<double>(dvalues, 3) << endl;

  return 0;
}
