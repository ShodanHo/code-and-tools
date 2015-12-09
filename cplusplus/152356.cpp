#include <iostream>
using namespace std;

int main()
{
  float x=0.375;

  if(x==0.375)
    cout<< "True\n";
  else
    cout<< "False\n";

  cout << sizeof(float) << endl;
  cout << sizeof(double) << endl;

  cout << sizeof(int) << endl;
  cout << sizeof(long long) << endl;

  cout << std::hex << "x    =" << *(int *)&x << endl; // sizeof(int) == sizeof(float) on my machine

  double d=0.375;
  cout << std::hex << "0.375=" << *(long long *)& d << endl; // sizeof(double) == sizeof(long long) on my machine
  d = (float)0.375;
  cout << std::hex << "d    =" << *(long long *)& d << endl;

  return 0;
}
