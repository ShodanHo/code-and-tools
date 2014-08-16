#include <iostream>
using namespace std;
  
class Test {
  static Test self;  // works fine
 
  /* other stuff in class*/
 
};

class Empty
{};
  
class Derived1 : public Empty
{};
  
class Derived2 : virtual public Empty
{};
  
class Derived3 : public Empty
{    
    char c;
};
  
class Derived4 : virtual public Empty
{
    char c;
};
  
class Dummy
{
    char c;
};

int& fun()
{
  int a = 10;
  return a;
}  

int main()
{
  int &x = fun();

  cout << "sizeof(Empty) " << sizeof(Empty) << endl;
  cout << "sizeof(Derived1) " << sizeof(Derived1) << endl;
  cout << "sizeof(Derived2) " << sizeof(Derived2) << endl;
  cout << "sizeof(Derived3) " << sizeof(Derived3) << endl;
  cout << "sizeof(Derived4) " << sizeof(Derived4) << endl;    
  cout << "sizeof(Dummy) " << sizeof(Dummy) << endl;
  
  cout << "x=" << x << endl;

  return 0;
}
