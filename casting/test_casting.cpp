#include <iostream>
#include <typeinfo>

using namespace std;

#define HERE() __FUNCTION__<< '(' << __LINE__ << "):"
#define STR(x) #x << '=' << x

class CBase {
public:
  virtual ~CBase() { std::cout << "CBase::~CBase()" << std::endl; }
  void func() { std::cout << HERE() << std::endl; }
};

class CDerived: public CBase {
public:
  ~CDerived() { std::cout << "CDerived::~CDerived()" << std::endl; }
  void func() { std::cout << HERE() << std::endl; }
};

static void print (char * str)
{
  cout << str << endl;
}

int main()
{
  std::cout << "hello" << std::endl;

  { // static_cast
    float pi = 3.14f;
    int i = static_cast<int>(pi);
    std::cout << HERE() << STR(pi) << std::endl;
    std::cout << HERE() << STR(i) << std::endl;
  }

  { // static_cast
    CBase * a = new CBase;
    //CDerived * b = a; // compile error
    CDerived * b = static_cast<CDerived*>(a);

    delete b;
  }

  std::cout << HERE() << std::endl;

  {
    CDerived * b = new CDerived;
    CBase * a = b;

    delete a;
  }

  std::cout << HERE() << std::endl;

  { // dynamic_cast
    CDerived d;
    CBase* pb = dynamic_cast<CBase*>(&d);
    if (pb == 0)
      cout << HERE() << "pb == 0" << endl;
    else
      pb->func();
  }

  std::cout << HERE() << std::endl;

  { // dynamic_cast
    CBase b;
    CDerived* pd = dynamic_cast<CDerived*>(&b); // runtime fail
    if (pd == 0)
      cout << HERE() << "pd == 0" << endl;
    else
      pd->func();
  }

  std::cout << HERE() << std::endl;

  { // dynamic_cast
    CBase * a = new CBase;
    CDerived * b = dynamic_cast<CDerived*>(a); // runtime fail
    if (b == 0)
      cout << HERE() << "b == 0" << endl;
    else
      delete b;
  }

  { // dynamic_cast
    class CBase { virtual void dummy() {} };
    class CDerived: public CBase { int a; };
    try {
      CBase * pba = new CDerived;
      CBase * pbb = new CBase;
      CDerived * pd;

      pd = dynamic_cast<CDerived*>(pba); // runtime pass
      if (pd==0) cout << "Null pointer on first type-cast" << endl;

      pd = dynamic_cast<CDerived*>(pbb); // runtime fail
      if (pd==0) cout << "Null pointer on second type-cast" << endl;
      
    } catch (exception& e) {cout << "Exception: " << e.what();}
  }

  { // reinterpret_cast
    class A {};
    class B {};
    A * a = new A;
    B * b = reinterpret_cast<B*>(a);
    delete b;
  }

  { // const_cast
    const char * c = "sample text";
    // print ( c ); // compile error
    print ( const_cast<char *> (c) );
  }

  { // typeid
    int * a,b;
    a=0; b=0;
    if (typeid(a) != typeid(b))
      {
	cout << "a and b are of different types:\n";
	cout << "a is: " << typeid(a).name() << '\n';
	cout << "b is: " << typeid(b).name() << '\n';
      }
  }

  { // typeid
    try {
      CBase* a = new CBase;
      CBase* b = new CDerived;
      cout << "a is: " << typeid(a).name() << '\n';
      cout << "b is: " << typeid(b).name() << '\n';
      cout << "*a is: " << typeid(*a).name() << '\n';
      cout << "*b is: " << typeid(*b).name() << '\n';
    } catch (exception& e) { cout << "Exception: " << e.what() << endl; }
  }

  {
    class CBase { virtual void dummy() {} };
    class CDerived: public CBase { int a; };

    try {
      CBase * pba = new CDerived;
      CBase * pbb = new CBase;
      CDerived * pd;

      pd = dynamic_cast<CDerived*>(pba);
      if (pd==0) cout << "Null pointer on first type-cast" << endl;

      pd = dynamic_cast<CDerived*>(pbb);
      if (pd==0) cout << "Null pointer on second type-cast" << endl;

    } catch (exception& e) {cout << "Exception: " << e.what();}
  }

  return 0;
}
