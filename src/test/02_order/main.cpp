//
// Created by Admin on 4/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

struct A {};

struct B {};

struct C {};

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.Request(
        [w = schedule.GetWorld()](Entity e, const A* a, const B* b) {
          w->AddCommand([e, w]() {
            if (!w->entityMngr.Have<C>(e)) {
              cout << "Attach C" << endl;
              w->entityMngr.Attach<C>(e);
            }
          });
        },
        "AB");
    schedule.Request(
        [w = schedule.GetWorld()](Entity e, const A* a, const B* b,
                                  const C* c) {
          w->AddCommand([e, w]() {
            if (w->entityMngr.Have<C>(e)) {
              cout << "Dettach C" << endl;
              w->entityMngr.Detach<C>(e);
            }
          });
        },
        "ABC");
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  w.entityMngr.CreateEntity<A, B>();

  w.Update();
  w.Update();
  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
