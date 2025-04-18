#include <MyECS/MyECS.hpp>
#include <iostream>

using namespace Smkz::MyECS;
using namespace std;

struct alignas(1) A {
  A() { cout << "A: " << this << endl; }
  char pad[13];
};
struct alignas(4) B {
  B() { cout << "B: " << this << endl; }
  char pad[28];
};
struct alignas(1) C {
  C() { cout << "C: " << this << endl; }
  char pad[15];
};
struct alignas(2) D {
  D() { cout << "D: " << this << endl; }
  char pad[10];
};
struct alignas(8) E {
  E() { cout << "E: " << this << endl; }
  char pad[24];
};
struct alignas(16) F {
  F() { cout << "F: " << this << endl; }
  char pad[32];
};
struct alignas(256) G {
  G() { cout << "G: " << this << endl; }
  char pad[256];
};

int main() {
  World w;
  w.entityMngr.cmptTraits.Register<A, B, C, D, E, F, G>();
  w.entityMngr.Create(Smkz::TypeIDs_of<A, B, C, D, E, F, G>);
  return 0;
}
