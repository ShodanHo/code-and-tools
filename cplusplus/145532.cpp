  /* vectormat.cpp */

#include <iostream>
//#include <conio>

class matrix; 

class vector
{
  static int n; 
  float *v; 
public:
  vector();
  vector(float *);
  vector(const vector &);
  ~vector();
  void display();
  static int & Size(){return n;};
  friend vector operator*(matrix &, vector &);
  friend class matrix;
};

int vector::n=0;


vector::vector()
{
  int i;
  v=new float[n];
  for(i=0;i<n;i++)
  {
    std::cout<<"The element is "<<(i+1)<<" : ";
    std::cin>>v[i];
  }
}

vector::vector(float *a)
{
  for(int i=0;i<n;i++)
    v[i]=a[i];
}

vector::vector(const vector &b)
{
  int i;
  for (i=0;i<n;i++)
    v[i]=b.v[i];
}

vector::~vector()
{
  delete v;
}

void vector::display() 
{
  for(int i=0;i<n;i++)
    std::cout<<v[i]<<" ";
  std::cout<<"\n";
}

// lop matrix
class matrix
{
   static int n; 
   vector *m; 
public:
   matrix();
   matrix(matrix &); 
   ~matrix();
   void display();
   static int &Size() {return n;};
   friend vector operator*(matrix &, vector &);
};

int matrix::n=0;


matrix::matrix()
{
  //	int i;
  m=new vector[n];
}

matrix::matrix(matrix &b)
{
  int i,j;
  m=new vector[n];
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      m[i].v[j]=b.m[i].v[j];
}

matrix::~matrix()
{
  delete m;
}

void matrix::display()
{
  for(int i=0;i<n;i++)
    m[i].display();
}

// operator * function matrix and vector
vector operator*(matrix &m,vector &v)
{
  float *a= new float[vector::Size()];
  int i,j;
  for(i=0;i<matrix::Size();i++)
  {
    a[i]=0;
    for(j=0;j<vector::Size();j++)
      a[i]+=m.m[i].v[j]*v.v[j];
  }
  return vector(a);
  //return a;
}

int main()
{
  //clrscr();
  int size;
  std::cout<<"Distance of vector "; std::cin>> size;
  vector::Size()=size;
  matrix::Size()=size;
  std::cout<<"Create a vector \n";
  vector v;
  std::cout<<" v= \n";
  v.display();
  std::cout<<"Create a matrix \n";
  matrix m;
  std::cout<<" m= \n";
  m.display();
  std::cout<<" Result of m*v is: \n";
  vector u=m*v; // operator m*v
  //u.display();
  //getch();
}
