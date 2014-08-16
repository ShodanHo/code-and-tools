#include <iostream>
using namespace std;

class SomeResource {
  int a;
public:
  SomeResource() {
    std::cout << "creating SomeResource!\n";
  }
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
  ~Example() {
    std::cout << "Deleting Example, freeing SomeResource!\n";
    delete p_;
  }
};

int main() {
  Example e1;
}
