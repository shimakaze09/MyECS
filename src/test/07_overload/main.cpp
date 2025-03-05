//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

struct P {};
struct V {};
struct A {};

struct VP_System {
  static void OnUpdate(Schedule& schedule) {
    schedule.Request([](const V*, P*) {cout << "VP" << endl; }, "VP");
  }
};

struct AVP_System {
  static void OnUpdate(Schedule& schedule) {
    schedule
        .Request([](const A*, V*, P*) {cout << "AVP" << endl; }, "AVP")
        .InsertNone<A>("VP");
  }
};

int main() {
  World w;
  w.systemMngr.Register<VP_System, AVP_System>();

  w.entityMngr.CreateEntity<V, P>();
  w.entityMngr.CreateEntity<A, V, P>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
