/*************************************************************************/
/*                                                                       */
/* Project: C++ investigation.                                           */
/*                                                                       */
/* Description: Test operators, specifically streaming.                  */
/*                                                                       */
/* Date: March 22, 2003.                                                 */
/*                                                                       */
/*************************************************************************/

/*
 * $Id$
 *
 * $Log$
 */

#include <iostream>

using namespace std;

#define TOSTR(x) #x << '=' << x.toString()

template <typename T, size_t Length>
size_t array_length(const T[Length])
{
  return Length;
}

#if 0
class complex {
  double real;
  double imag;
public:
  complex() : real(0.0), imag(0.0) {}
};
#endif

#if 0
template <double* pd>
class X {
  double real;
  double imag;
public:
  X() : real(*pd), imag(0.0) {}
};
#endif

template <typename T, T* initial>
class complex {
  T real;
  T imag;
public:
  complex() : real(*initial), imag(*initial) {}
  complex(const T& r, const T& i) : real(r), imag(i) {}

  complex operator+() const;
  complex operator+(const complex& x) const;
  complex& operator+=(const complex& x);

  complex operator-() const;
  complex operator-(const complex& x) const;
  complex& operator-=(const complex& x);

  std::string toString() const;
};

template <typename T, T* i>
complex<T,i> complex<T,i>::operator+() const
{
  return *this;
}

template <typename T, T* i>
complex<T,i> complex<T,i>::operator+(const complex& x) const
{
  return complex(real+x.real,imag+x.imag);
}

template <typename T, T* i>
complex<T,i>& complex<T,i>::operator+=(const complex<T,i>& x)
{
  real += x.real;
  imag += x.imag;
  return *this;
}

template <typename T, T* i>
complex<T,i> complex<T,i>::operator-() const
{
  return complex<T,i>(-real,-imag);
}

template <typename T, T* i>
complex<T,i> complex<T,i>::operator-(const complex& x) const
{
  return complex(real-x.real,imag-x.imag);
}

template <typename T, T* i>
complex<T,i>& complex<T,i>::operator-=(const complex<T,i>& x)
{
  real -= x.real;
  imag -= x.imag;
  return *this;
}

template <typename T, T* i>
std::string
complex<T,i>::toString() const
{
  char str[20];
  sprintf(str, "(%lf,%lf)", real, imag);
  return std::string(str);
}

#if 0
template <typename T, T* initial>
complex& complex::operator+(const complex&)
{
}
#endif

static double double_zero = 0.0;

int
main()
{
  cout << __FILE__ << " start" << endl;

  complex<double,&double_zero> x(1.0,0.0);
  complex<double,&double_zero> y(0.0,1.0);
  complex<double,&double_zero> z;
  std::cout << TOSTR(z) << std::endl;
  z = x + y;

  std::cout << TOSTR(x) << std::endl;
  std::cout << TOSTR(y) << std::endl;
  std::cout << TOSTR(z) << std::endl;

  z = +z;
  std::cout << TOSTR(z) << std::endl;

  z = -z;
  std::cout << TOSTR(z) << std::endl;

  z = x - y;
  std::cout << TOSTR(z) << std::endl;

  z = x;
  z += y;
  std::cout << TOSTR(z) << std::endl;

  z = x;
  z -= y;
  std::cout << TOSTR(z) << std::endl;

  cout << __FILE__ << " end" << endl;
  return 0;
}

// End of file
