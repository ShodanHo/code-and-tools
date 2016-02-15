/*************************************************************************/
/*                                                                       */
/* Project: Standard Template Library.                                   */
/*                                                                       */
/* Description: Playing with the algorithms in the STL.                  */
/*                                                                       */
/* Date: September 10, 2011.                                             */
/*                                                                       */
/*************************************************************************/

/*
 * $Id: test_for_each.cpp,v 1.2 2013/09/23 00:18:33 knoppix Exp $
 *
 * $Log: test_for_each.cpp,v $
 * Revision 1.2  2013/09/23 00:18:33  knoppix
 * Comment
 *
 * Revision 1.1  2013/09/21 18:01:48  knoppix
 * Original
 *
 */

#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

#define STR(x) #x << '=' << x << ' '

template<class T>
ostream&
operator<<(ostream& os, const list<T>& t)
{
  typename list<T>::const_iterator it;
  for (it = t.begin(); it != t.end();) {
    os << *it;
    if (++it != t.end())
      os << ",";
  }
  return os;
}

/**
 * structure to keep track of instances of itself.
 */
struct adder_t
{
  static int instances;
  int instance;
  int total;
  adder_t() : instance(instances++), total(0) {
    cout << __FUNCTION__ << " normal constructor "
	 << STR(instance) << endl;
  }

  adder_t(const adder_t& a) : instance(instances++), total(a.total) {
    cout << __FUNCTION__ << " copy constructor from instance="
	 << a.instance << " to "
	 << STR(instance) << endl;
  }

  ~adder_t() {
    cout << __FUNCTION__ << ' ' << STR(instance) << endl;
  }

  void operator()(int i) {
    total += i;
    cout << __FUNCTION__ << ' '
	 << STR(instance) << STR(i) << STR(total) << endl;
  }
  int Get(void) const
  {
    cout << __FUNCTION__ << ' ' << STR(instance) << STR(total) << endl;
    return total;
  }
};

int adder_t::instances = 0;

class scope_definition_t
{
private:
  int instance;
public:
  scope_definition_t(int _instance) : instance(_instance) {
    cout << "--- start " << instance << " ---" << endl; }
  ~scope_definition_t() {
    cout << "--- stop  " << instance << " ---" << endl; }
};

#define SCOPE() scope_definition_t x(__LINE__)

int
main(int argc, char **argv)
{
  cout << __FILE__ << " start\n";

  list<int> int_list;
  int_list.push_back(1);
  int_list.push_back(2);
  int_list.push_back(3);
  cout << STR(int_list) << endl;

  {
    SCOPE();

    adder_t adder;
 
   for_each(int_list.begin(), int_list.end(), adder);

    cout << STR(adder.Get()) << endl;
  }

  {
    SCOPE();
    adder_t adder;

    list<int>::const_iterator it;

    for(it = int_list.begin(); it != int_list.end(); it++) {
      adder(*it);
    }

    cout << STR(adder.Get()) << endl;

  }

  {
    SCOPE();
    adder_t adder = for_each(int_list.begin(), int_list.end(), adder_t());

    cout << STR(adder.Get()) << endl;
  }

  {
    SCOPE();
    cout << "result="
	 << for_each(int_list.begin(), int_list.end(), adder_t()).Get()
	 << endl;
  }

  cout << __FILE__ << " end\n";

  return 0;
}

// End of file
