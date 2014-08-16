#include <iostream>
using namespace std;

struct A {
  void f() { cout << "A::" << __FUNCTION__ << endl; }
};

struct B: A {
  void f() { cout << "B::" << __FUNCTION__ << endl; }
};

static void g(A& arg) {
   arg.f();
}

static void g(A* arg) {
   arg->f();
}

struct C {
  virtual void f() { cout << "Class C" << endl; }
};

struct D: C {
  void f() { cout << "Class D" << endl; } // this is virtual
};

struct E: D {
  void f() { cout << "Class E" << endl; } // this is virtual
};

static void f(C arg) {
   arg.f();
}

static void g(C& arg) {
   arg.f();
}

static void h(D& arg) {
   arg.f();
}

int main() {
   B x;
   g(x);
   g(&x);

   D d;
   f(d);
   g(d);
   h(d);

   C z;
   g(z);

   C* cptr = &d;
   cptr->f();

   E e;
   cptr = &e;
   cptr->f();
   {
     struct A { virtual void f() { std::cout << "A::f()" << std::endl; } };
struct B : A { };
struct C : A { void f() { std::cout << "C::f()" << std::endl; } };
struct D : B, C { }; // OK: A::f and C::f are the final overriders
                     // for the B and C subobjects, respectively

 D d;
 d.D::B::f();
 d.C::f();
   }
}
