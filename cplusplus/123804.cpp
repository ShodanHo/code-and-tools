#include <iostream>

class myComplex{
	
  double real;
  double imag;
	
public:
  myComplex(){};
  myComplex(double r, double i) : real(r), imag(i){};
  myComplex operator+ (const myComplex& in);
  myComplex operator- (const myComplex& in);
  myComplex operator* (const myComplex& in);
		
  friend std::ostream &operator<<(std::ostream &out, myComplex c);
  friend std::istream &operator>>(std::istream &in, myComplex &c);
};

myComplex myComplex:: operator+ (const myComplex& in){
	double result_real = real + in.real;
	double result_imag = imag + in.imag;
	return myComplex (result_real, result_imag);
}

myComplex myComplex:: operator- (const myComplex& in){
	double result_real = real - in.real;
	double result_imag = imag - in.imag;
	return myComplex (result_real, result_imag);
}

myComplex myComplex:: operator* (const myComplex& in){
	double result_real = real * in.real;
	double result_imag = imag * in.imag;
	return myComplex (result_real, result_imag);
}

std::ostream &operator<<(std::ostream &out, myComplex c)    
{
        out<<"real part: "<<c.real<<"\n";
        out<<"imag part: "<<c.imag<<"\n";
        return out;
}

std::istream &operator>>(std::istream &in, myComplex &c)     
{
        std::cout<<"enter real part:\n";
        in>>c.real;
        std::cout<<"enter imag part: \n";
        in>>c.imag;
        return in;
}

int main()
{
  using namespace std;
  myComplex a;
  cin>>a;
  cout<<a;
}
