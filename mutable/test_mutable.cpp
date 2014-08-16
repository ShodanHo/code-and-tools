/*
 * ref: http://stackoverflow.com/questions/105014/does-the-mutable-keyword-have-any-purpose-other-than-allowing-the-variable-to
 */
#include <iostream>

class A {
  mutable int a;
public:
  void func() const { a = 1; }
  A() : a(0) {}
};

class Employee {
  const std::string _name;
public:
  Employee(std::string name) : _name(name) {}
  void setName(const std::string& name);
};

void
Employee::setName(const std::string& name)
{
  //_name = name; // this causes error
}

int main()
{
  std::cout << "hello" << std::endl;
  return 0;
}
