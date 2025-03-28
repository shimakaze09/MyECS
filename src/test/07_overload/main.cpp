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

struct VP_System {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob([](const V*, P*) { cout << "VP" << endl; },
                               "VP");
  }
};

struct AVP_System {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob([](const A*, V*, P*) { cout << "AVP" << endl; },
                               "AVP");
    schedule.InsertNone("VP", CmptType::Of<A>);
  }
};

int main() {
  World w;
  auto systemIndices = w.systemMngr.systemTraits.Register<VP_System, AVP_System>();
  for (auto idx : systemIndices)
    w.systemMngr.Activate(idx);

  w.entityMngr.Create<V, P>();
  w.entityMngr.Create<A, V, P>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
