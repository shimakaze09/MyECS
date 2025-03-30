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

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob(
        [](Entity e, const A* a) { cout << e.Idx() << ": " << a->val << endl; },
        "MySystem", false);
  }
};

int main() {
  World w;
  w.systemMngr.RegisterAndActivate<MySystem>();

  auto [e] = w.entityMngr.Create<>();
  w.entityMngr.Emplace<A>(e, 1.f);
  w.entityMngr.Instantiate(e);
  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
