//
// Created by Admin on 28/02/2025.
//

#include <MyECS/CmptTag.h>
#include <MyECS/World.h>

#include <iostream>

using namespace std;
using namespace My;

struct A {
  void OnUpdate() const { cout << "A" << endl; }
};

struct B {
  void OnUpdate(CmptTag::Newest<A> a) { cout << "B" << endl; }
};

struct C {
  void OnUpdate(CmptTag::LastFrame<B> b, CmptTag::Write<A> a) const {
    cout << "C" << endl;
  }
};

int main() {
  CmptRegistrar::Instance().Register<A, B, C>();

  World w;
  w.CreateEntity<A, B, C>();

  w.Start();
  w.Update();
  w.Update();
  w.Stop();

  cout << w.DumpUpdateTaskflow() << endl;

  return 0;
}
