#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace My::MyECS;
using namespace std;

struct A {};

struct B {};

struct C {};

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob(
        [](World* w, Entity e, const A* a, const B* b) {
          w->AddCommand([e, w]() {
            if (!w->entityMngr.Have(e, TypeID_of<C>)) {
              cout << "Attach C" << endl;
              w->entityMngr.Attach(e, TypeIDs_of<C>);
            }
          });
        },
        "AB");
    schedule.RegisterEntityJob(
        [](World* w, Entity e, const A* a, const B* b, const C* c) {
          w->AddCommand([e, w]() {
            if (w->entityMngr.Have(e, TypeID_of<C>)) {
              cout << "Detach C" << endl;
              w->entityMngr.Detach(e, TypeIDs_of<C>);
            }
          });
        },
        "ABC");
  }
};

int main() {
  World w;
  w.entityMngr.cmptTraits.Register<A, B, C>();
  w.systemMngr.RegisterAndActivate<MySystem>();

  w.entityMngr.Create(My::TypeIDs_of<A, B>);

  w.Update();
  w.Update();
  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
