#include <iostream>
using namespace std;

class SomeResource {
  int a;
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
}
