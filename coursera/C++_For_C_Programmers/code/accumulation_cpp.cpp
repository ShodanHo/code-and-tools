//
// homework assignment 1. Sum over an array.
//
// (1) change to C++ io
// (2) change to one line comments
// (3) change defines of constants to const
// (4) change array to vector<>
// (5) inline any short function
//

#include <iostream>
#include <vector>

// set all elements in a vector to an incrementing sequence
// named after std::iota without iterators
template <class T>
inline void iota(std::vector<T>& vec, // the vector
		 const T& initial, // the value to set the first element to
		 const T& delta) // difference between successive values
{
  T value = initial; // start with the initial value

  for (T& t: vec) { // loop over each element in the vector
    t = value; // set the element in the vector
    value += delta; // increment
  }
}

// return the sum of all elements in a vector
template <class T>
inline T summation(const std::vector<T>& v)
{
  T sum = 0; // accumulate here <-- this line does not work for more complex T

  for (const T& t: v) // loop over each element in the vector
    sum += t; // sum each element

  return sum; // return the summation
}

int main()
{
  const int vector_size = 40; // elements in our vector
  std::vector<int> data(vector_size); // construct vector with elements

  iota(data, 0, 1); // set vector to 0,1,2, ... N-1

  int sum = summation(data); // sum over all the elements

  std::cout << "sum is " << sum << std::endl; // print the result

  return 0;
}
