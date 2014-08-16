/*************************************************************************/
/*                                                                       */
/* Project: C++ investigation.                                           */
/*                                                                       */
/* Description: Test structures, specifically labelled initialisers.     */
/*                                                                       */
/* Date: February 29, 2004.                                              */
/*                                                                       */
/*************************************************************************/

/*
 * $Id$
 *
 * $Log$
 */

#include <iostream>

#define STR(x) #x << '=' << x << ' '

using namespace std;

struct xyz_t
{
  int a;
  int b;
  float f;
};

ostream &
operator<<(ostream &os, const xyz_t &xyz)
{
  os << "a=" << xyz.a << " b=" << xyz.b << " f=" << xyz.f;
  return os;
}

int
main()
{
  cout << __FILE__ << " start" << endl;

  xyz_t xyz = {
    a: 1,
    b: 2,
    f: 2.0,
  };
  cout << STR(xyz) << endl;

  cout << __FILE__ << " end" << endl;
  return 0;
}

// End of file
