#include <iostream>
#include <sstream>

using namespace std;

#define STR(x) #x << '=' << x

void dealloc(double ****p, int n)
{
  for (int i = 0; i<n; i++)
    delete[](**p)[i];
  delete[] *p;	//1
}
int init(double **p, int n)
{
  for (int i = 0; i<n; i++)
    for (int j = 0; j<n; j++)
      p[i][j] = i + j;
  return n;
}
std::string toString(double **p, int n)
{
  std::ostringstream oss;
  for (int i = 0; i<n; i++) {
    std::cout << i << ':' << p[i] << "\n";
    for (int j = 0; j<n; j++) {
      std::cout << i << '.' << j << ':' << p[i][j] << "\n";
    }
  }
  return oss.str();
}
int alloc(double **&p)	//2
{
  const int size = 10;
  p = new double*[size];	//3
  std::cout << STR(p) << "\n";
  for (int i = 0; i<size; i++) {
    p[i] = new double[size];
    std::cout << "p[" << i << "]=" << p[i] << "\n";
  }
  return size;
}
int main(int argc, char* argv[])
{
  double **A;
  double ***p = &A;
  //alloc(A);	//4
  //std::cout << STR(A) << "\n";
  alloc(A);	//4
  toString(A, 10);
  init(A, 10);	//4
  toString(A, 10);
  dealloc(&p, 10);
  std::cout << STR(A) << "\n";
  //dealloc(&p, init(A, alloc(A)));	//4
  return 0;
}
