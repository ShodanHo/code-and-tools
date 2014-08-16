#include<iostream>
#include<cmath>
using namespace std;

#define STR(x) #x << '=' << x

int main ()
{
  double intervals=(3.5/2.0)-3.0;
  intervals=(3.5/2.0) - 3.0;
  double tempPerMin=exp(intervals)/(1+exp(intervals));
  tempPerMin=1.0;
  double naturalChange=(-0.05)*tempPerMin*3.5;
  //naturalChange=(-0.05)*tempPerMin;
  double coolChange=(0.1)*(tempPerMin)*3.5;
  //coolChange=(0.1)*(tempPerMin);
  double heatChange=(0.125)*(tempPerMin)*3.5;
  //heatChange=(0.125)*(tempPerMin);
  double tempAtEnd=38.00-coolChange+naturalChange+heatChange;

  cout << STR(intervals) << endl;
  cout << STR(tempPerMin) << endl;
  cout << STR(naturalChange) << endl;
  cout << STR(coolChange) << endl;
  cout << STR(heatChange) << endl;
  cout << STR(tempAtEnd) << endl;

  cout<<tempAtEnd << endl;

    return 0;

}
