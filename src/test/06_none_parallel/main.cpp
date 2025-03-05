//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

struct A {};

struct B {};

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    EntityFilter filter_w0(TypeList<>{},  // all
                           TypeList<>{},  // any
                           TypeList<A>{}  // none
    );
    EntityFilter filter_w1(TypeList<A>{},  // all
                           TypeList<>{},   // any
                           TypeList<>{}    // none
    );
    schedule.Request([](B*) {}, "need B, none A", filter_w0)
        .Request([](B*) {}, "need A, B", filter_w1);
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();
  w.entityMngr.CreateEntity<>();
  w.entityMngr.CreateEntity<A>();
  w.entityMngr.CreateEntity<B>();
  w.entityMngr.CreateEntity<A, B>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
