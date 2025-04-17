#include <MyECS/MyECS.hpp>
#include <iostream>

using namespace My::MyECS;
using namespace std;

struct A {
  float val;
};

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob(
        [](Entity e, const A* a) { cout << e.index << ": " << a->val << endl; },
        "MySystem", false);
  }
};

int main() {
  World w;
  w.entityMngr.cmptTraits.Register<A>();
  w.systemMngr.RegisterAndActivate<MySystem>();

  auto e = w.entityMngr.Create(My::TypeIDs_of<A>);
  w.entityMngr.WriteComponent<A>(e)->val = 1.f;
  w.entityMngr.Instantiate(e);
  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
