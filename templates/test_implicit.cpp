#include "implicit.h"
#include <vector>
#include <list>
#include <map>
using namespace std;

#define STR(x) #x << '=' << x

int main()
{
  {
    vector<int> vector;
    cout << STR(vector) << endl;
  }
  {
    vector<int> vector(10);
    for (auto& v: vector) {
      static int i = 0;
      v = i++;
    }

    cout << STR(vector) << endl;
  }

  {
    list<int> list;
    cout << STR(list) << endl;
  }
  {
    list<int> list(10);
    for (auto& l: list) {
      static int i = 0;
      l = i++;
    }

    cout << STR(list) << endl;
  }

  {
    map<int,string> map;
    cout << STR(map) << endl;
  }
  {
    map<int,string> map;
    string str[] = { "zero", "one", "two", "three", "four",
		     "five", "six", "seven", "eight", "nine" };
    for (auto& s: str) {
      static int i = 0;
      map[i] = s;
      ++i;
    }

    cout << STR(map) << endl;
  }

  return 0;
}
