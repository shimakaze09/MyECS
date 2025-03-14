//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;
using namespace std;

struct P {};

struct V {};

struct A {};

class VP_System : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    schedule.RegisterEntityJob([](const V*, P*) { cout << "VP" << endl; },
                               "VP");
  }
};

class AVP_System : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    schedule.RegisterEntityJob([](const A*, V*, P*) { cout << "AVP" << endl; },
                               "AVP");
    schedule.InsertNone("VP", CmptType::Of<A>);
  }
};

int main() {
  World w;
  w.systemMngr.Register<VP_System, AVP_System>();

  w.entityMngr.Create<V, P>();
  w.entityMngr.Create<A, V, P>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
