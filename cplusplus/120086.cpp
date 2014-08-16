#include <iostream>
#include <stack>
#include <sstream>

using namespace std;

#define STR(x) #x << '=' << x

typedef std::stack<double> math_stack;

std::ostream&
operator<<(std::ostream& os, const math_stack& ms)
{
  std::stack<double> s(ms);
  while (!s.empty()) {
    os << s.top();
    s.pop();
    if (!s.empty())
      os << ',';
  }
  return os;
}

bool char_operator(const std::string &input)
{
  std::string ops[] = {"+","-","*","/"};
  for (int i = 0; i < 4; i++)
  {
    if(input == ops[i])
    {
      return true;
    }
  }
  return false;
}

double perform_operation(const std::string& input, math_stack& calc_stack)
{
  double val1, val2, result;
  val2 = calc_stack.top();
  calc_stack.pop();
  val1 = calc_stack.top();
  calc_stack.pop();
  if (input == "+")
  {
    result = val1 + val2;
  }
  else if (input == "-")
  {
    result = val1 - val2;
  }
  else if (input == "*")
  {
    result = val1 * val2;
  }
  else
  {
    result = val1 / val2;
  }
  calc_stack.push(result);
  return result;
}

double simple_expr(const std::string& expression, math_stack& num_stack)
{
  std::cout << STR(expression) << std::endl;

  double numerical;

  std::istringstream x(expression);
  x >> numerical;
  if (!x.fail())
  {
    num_stack.push(numerical);
    return numerical;
  }

  std::cout << "not a number" << std::endl;

  if (char_operator(expression))
  {
    std::cout << "an operation" << std::endl;
    double returning;
    returning = perform_operation(expression, num_stack);
    return returning;
  }
  std::cout << "not an operation" << std::endl;
  return -1;
}

int main()
{
  string expression;
  double result;

  math_stack num_stack;
  cout <<  " RPN Calculator - Lucas Henrique, C++\n Hit \'q\' and Enter to quit"
       <<endl;
  while(true) {
    std::cout << STR(num_stack) << std::endl;
    cout<<"> ";
    cin>>expression;
    if(expression == "q")
    {
      std::cout<<" Exiting. Bye!";
      return 0;
    }
    else
    {
      result = simple_expr(expression, num_stack);
      cout<<result<<endl;
    }
  }
}
