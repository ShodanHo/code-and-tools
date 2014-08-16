#include <iostream>
using namespace std;

#define HERE() __FUNCTION__<< '(' << __LINE__ << "):"
#define STR(x) #x << '=' << x

class SomeResource {
public:
  int _a;
  SomeResource(int a) : _a(a) {
    std::cout << "creating SomeResource! " << STR(_a) << "\n";
  }
  ~SomeResource() {
    std::cout << "deleting SomeResource! " << STR(_a) << "\n";
  }
};

class Example {
  SomeResource* p_;
public:
  Example(int a) : p_(new SomeResource(a)) {
    std::cout << "Creating Example, allocating SomeResource! "
	      << STR(a) << "\n";
  }
  Example(const Example& e) : p_(new SomeResource(e.p_->_a + 1)) {
    std::cout << "Copying Example, allocating SomeResource!\n";
  }
  Example& operator=(const Example& e) {
    std::cout << "Assigning Example!\n";
    if (this == &e)
      return *this;
    *p_ = *e.p_;
    return *this;
  }
  ~Example() {
    std::cout << "Deleting Example, freeing SomeResource!\n";
    delete p_;
  }
};

int main() {
  Example e1(1); // constructor
  Example e2(e1); // copy constructor
  e1 = e2; // assignment operator
}
