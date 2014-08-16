#include <iostream>
using namespace std;

#define HERE() __FUNCTION__<< '(' << __LINE__ << "):"
#define STR(x) #x << '=' << x

class SomeResource {
  int a;
public:
  ~SomeResource() {
    std::cout << "deleting SomeResource!\n";
  }
};

class Example {
  SomeResource* p_;
public:
  Example() : p_(new SomeResource()) {
    std::cout << "Creating Example, allocating SomeResource!\n";
  }
  Example(const Example& e) : p_(new SomeResource()) {
    std::cout << "Copying Example, allocating SomeResource!\n";
  }
  ~Example() {
    std::cout << "Deleting Example, freeing SomeResource!\n";
    delete p_;
  }
};

int main() {
  Example e1;
  Example e2(e1); // copy constructor
  cout << HERE() << endl;
  e2 = e1;
  cout << HERE() << endl;
}
