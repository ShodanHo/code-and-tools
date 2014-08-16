#include <iostream>
#include <list>
#include <limits>
using namespace std;

static float power(float x, int y)
{
  float z=1;
  for (int i = 1; i <= y; ++i) {
    z *= x;
  }
  return z;
}

static float ploynomial(const list<pair<int, float> >& pow_coeffs, float x)
{
  float result = 0.0;
  for (auto &cp: pow_coeffs) {
    int pow = cp.first;
    float coeff = cp.second;
    result += coeff * power(x, pow);
    //cout << "pow=" << pow << " coeff=" << coeff << " result=" << result << endl;
  }
  //cout << "result=" << result << endl;
  return result;
}

ostream &operator << (ostream &oss, list<pair<int, float> > &lst)
{
  for (auto &w : lst)
    oss << '(' << w.first << "," << w.second << ')';
  return oss;
}

int main()
{
  cout << "Enter the highest exponent of your function: ";

  int hiPow;
  while (!(cin >> hiPow) || hiPow < 0) {
    if( hiPow < 0 )
      cout << "\n\nThis exponent is negative. \n\nPlease enter a positive integer now: ";
    else { //bad input
      cin.clear(); //clear bad input flag
      cin.ignore(numeric_limits<streamsize>::max(), '\n');// clear stream buffer
    }
  }

  list<pair<int, float> > pow_coeffs;
  for (int i = hiPow; i > 0; --i){
    cout << "Enter the coefficient of the x^" << i <<" term: ";
    float coef;
    cin >>coef;
    pow_coeffs.push_back(pair<int,float>(i, coef));
  }
  cout << "Enter the constant number for your function: ";
  float con; cin >> con;
  pow_coeffs.push_back(pair<int,float>(0, con));

  cout << "pow_coeffs=" << pow_coeffs << endl;

  list<float> X; // all the x's we want to compute f(x) for
  char c;
  do {
    cout << "Enter the value of x: ";
    float x; cin >> x; X.push_back(x);
    cout<< "Do you have another value for x you want calculated?\n"
	<< "Enter 1 for yes 0 for no: ";
    cin >> c;
    c = tolower(c);
  } while( c == 'y' || c == '1' );

  // compute each f(x)
  for (auto &x : X) {
    float result = ploynomial(pow_coeffs, x);
    cout << "f("<<x<<") = "<< result << endl;
  }

  return 0;
}
