//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;
using namespace std;

struct A {
  float val;
};

class MySystem : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    schedule.Register(
        [](Entity e, const A* a) { cout << e.Idx() << ": " << a->val << endl; },
        "");
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  auto [e] = w.entityMngr.Create<>();
  w.entityMngr.Emplace<A>(e, 1.f);
  w.entityMngr.Instantiate(e);

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
