#include <fstream>
#include <iostream>
using namespace std;

int main( )
{
  float distance1 = 2.5 ;
  float distance2 = 4.3 ;
  float distance3 = 6.2 ;
  float distance4 = 8.1 ;
  float scale = 0.0001 ;
  ofstream outData;

  outData.open("E:\\walk.dat");

  outData << distance1 << distance2 << distance3 << distance4 << scale;

  outData.close( );
  //system("pause");

  return 0;
}
