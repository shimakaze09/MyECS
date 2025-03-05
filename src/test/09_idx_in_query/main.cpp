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
    auto flags = std::make_shared<std::vector<bool>>();
    schedule
        .Request([flags](Entity e, size_t indexInQuery,
                         const A*) { flags->at(indexInQuery) = true; },
                 "set flag")
        .Request(
            [flags]() {
              for (auto flag : *flags)
                cout << flag << endl;
            },
            "print flag")
        .Order("set flag", "print flag");
    size_t num = schedule.EntityNumInQuery("set flag");
    flags->insert(flags->begin(), num, false);
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  w.entityMngr.CreateEntity<A>();
  w.entityMngr.CreateEntity<A>();
  w.entityMngr.CreateEntity<A, B>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
