/** 
 * binaryConvert.cpp
 * This program converts binary numbers into 
 *the equivalent decimal value.
 * <p>
 *
 * author: Jordan Warnecke
 * modified by: N/A
 * email: jwarneck@umw.edu
 * language: C++
 * pledge: I pledge. JHW 10/11/2013
 *
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
#define STR(x) #x << '=' << x

static unsigned convertBinary(const string& str)
{
  unsigned result = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    if (str.at(i) == '0')
      result = 2 * result;
    else if (str.at(i) == '1')
      result = 2 * result + 1;
    else {
      cerr << "Bad digit on input." << endl;
    }
  }
  cout << STR(result) << endl;
  return result;
}

int main()
{
  char inputFileName[50]; //store file name entered by user
  cout <<"Enter the input file name" << endl;
  cin >> inputFileName; //read in user file name

  char outputFileName[50]; //store output file name
  cout <<"Enter file name where output is to be stored: " << endl;
  cin >> outputFileName;

  cout << STR(inputFileName) << ' ' << STR(outputFileName) << endl;

  ifstream fcin;
  fcin.open(inputFileName); // open the input file

  ofstream fcout;
  fcout.open(outputFileName); //open output file
  while (!fcin.eof()){
    string binary;
    fcin >> binary;
    cout << STR(binary) << endl;
    if (binary.size() == 0)
      continue; // ignore empty lines
    unsigned num = convertBinary (binary);
    cout << STR(num) << std::endl;
    fcout << num << endl;
  }
    
  fcin.close();
  fcout.close();
    
  return 0;
}
