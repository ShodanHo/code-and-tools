#include<iostream>
using namespace std;


class singleton
{
  int a ;

  static int count;
  //static singleton*ptr;

  singleton():a(0)
  {
    cout << "I am in constructor this=" << this << endl;
  }
  ~singleton()
  {
    cout << "I am in destructor this=" << this << endl;
  }
public: 
  void display()
  {
    cout<<"a is "<<a<<endl;

  }


  static singleton* createinstance()
  {
    if(count==0)
      { static singleton instance;
	return &instance;
      }

    else
      {
	cout<<"Sorry one instance is already created"<<endl;
	return NULL;
      }
  }

};

int singleton::count=0;
int main()
{
  {
    singleton*ptr=singleton::createinstance();

    ptr->display(); 
  }
  std::cout << __LINE__ << std::endl;
  return 0;
}
