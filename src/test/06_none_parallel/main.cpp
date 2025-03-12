//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;
using namespace My;
using namespace std;

struct A {};

struct B {};

class MySystem : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    EntityFilter filter_w0(TypeList<>{},  // all
                           TypeList<>{},  // any
                           TypeList<A>{}  // none
    );
    EntityFilter filter_w1(TypeList<A>{},  // all
                           TypeList<>{},   // any
                           TypeList<>{}    // none
    );
    schedule.Register([](B*) {}, "need B, none A", filter_w0)
        .Register([](B*) {}, "need A, B", filter_w1);
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();
  w.entityMngr.Create<>();
  w.entityMngr.Create<A>();
  w.entityMngr.Create<B>();
  w.entityMngr.Create<A, B>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
