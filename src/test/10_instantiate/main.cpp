//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

struct A { float val; };

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.Request([](Entity e, const A* a) {
        cout << e.Idx() << ": " << a->val << endl;
    }, "");
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  auto [e] = w.entityMngr.CreateEntity<>();
  w.entityMngr.Emplace<A>(e, 1.f);
  w.entityMngr.Instantiate(e);

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
