#include <iostream>
#include <fstream>
using namespace std;

#if 1
void readFile(char weather[][30], ifstream& inputFile)
{
  for(int i = 0; i < 3; i++)
  {
    for(int k = 0; k < 30; k++)
    {
      inputFile >> weather[i][k];
    }
  }
}
#else
void readFile(char weather[][30])
{
  for(int i = 0; i < 3; i++)
  {
    for(int k = 0; k < 30; k++)
    {
      //inputFile >> weather[i][k];
    }
  }
}
#endif

int main()
{
  char weather[3][30];
  ifstream inputFile("P5.txt");

#if 1
  readFile(weather, inputFile);
#else
  readFile(weather);
#endif

  for(int i = 0; i < 3; i++)
  {
    for(int k = 0; k < 30; k++)
    {
      cout << weather[i][k];
      if(k == 29)
      {
	cout << endl;
      }
    }
  }

  return 0;
}
