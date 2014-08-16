#include <iostream>
#include <list>
#include <fstream>
using namespace std;

// stream a pair
template<typename A, typename B>
ostream & operator<<(ostream& os, const pair<A,B>& p)
{
  os << '(' << p.first << ',' << p.second << ')';
  return os;
}

// stream a list
template<typename T>
ostream & operator<<(ostream& os, const list<T>& l)
{
  for (auto it = l.cbegin(); it != l.cend(); ++it) {
    os << *it;
  }
  return os;
}

// convert a roman token to a number. return 0 if invalid
static unsigned RomanTokenToDecimal(char c)
{
  switch (c) {
  case 'I': return 1;
  case 'V': return 5;
  case 'X': return 10;
  case 'L': return 50;
  case 'C': return 100;
  case 'D': return 500;
  case 'M': return 1000;
  }
  return 0;
}

/*
 * convert a string of Roman symbols to an unsigned number.
 * no checks are done on correctness of input
 * IV => 4
 * IVX => 4
 * VV => 10
 */
static unsigned RomanStringToNumber(const string& str)
{
  /* step 1: convert each token to a sign and a number
   * step 2: work out if there is subtraction of the number to take place
   * step 3: then calculate the number
   *
   * eg: MCMLIX:
   * step 1:
   * M: + 1000
   * C: + 100
   * M: + 1000
   * L: + 50
   * I: + 1
   * X: + 10
   * 
   * step 2:
   * M: + 1000
   * C: - 100 <--- subtract because next number is greater
   * M: + 1000
   * L: + 50
   * I: - 1 <--- subtract because next number is greater
   * X: + 10
   *
   * step3: +1000-100+1000+50-1+10=1990
   */
  list< pair<bool,unsigned> > sign_value; // true(+),false(-)
  // step 1
  for (auto &c: str) {
    sign_value.push_back( pair<bool,unsigned>(true, RomanTokenToDecimal(c)) );
  }

  // step 2: change the sign if the subsequent value is higher
  for (auto it = sign_value.begin(); it != sign_value.end(); ++it) {
    if (it != sign_value.begin()) {
      auto prev = it; --prev;
      if (prev->second < it->second) // do we need to subtract
	prev->first = false;
    }
  }

  // step 3: sum the numbers
  unsigned sum = 0;
  for (auto it = sign_value.cbegin(); it != sign_value.cend(); ++it) {
    sum = sum + (it->first ? it->second : -it->second);
  }

  //cout << sign_value << ' ' << sum << endl;
  return sum;
}

/*
 * not sure if romans had negative numbers, but allow negative numbers
 */
static int doOperation(unsigned arg1, unsigned arg2, char op)
{
  switch (op) {
  case '+': return arg1 + arg2;
  case '-': return arg1 - arg2;
  case '*': return arg1 * arg2;
  case '/': return arg1 / arg2;
  }
  return 0;
}

int main()
{
  // do a little bit of checking
  {
    string a("I");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("II");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("III");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("IV");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("V");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("VI");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("VII");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("VIII");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("IX");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("X");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }
  {
    string a("MCMXL");
    cout << a << ':' << RomanStringToNumber(a) << endl;
  }

  // open the file
  ifstream fin;
  fin.open("MP4.txt");

  // read line by line, performing the operation
  for (;;) {
    string rm1, rm2;
    char op;
    fin >> rm1 >> rm2 >> op;
    if (fin.fail()) break;

    cout << "rm1=" << rm1 << " rm2=" << rm2 << " op=" << op << " ";

    cout << RomanStringToNumber(rm1) << op
	 << RomanStringToNumber(rm2) << "="
	 << doOperation(RomanStringToNumber(rm1),
			RomanStringToNumber(rm2),op) << endl;
  }
  fin.close();
  return 0;
}
