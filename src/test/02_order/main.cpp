//
// Created by Admin on 4/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;
using namespace std;

struct Data1 {};

struct Data2 {};

struct DataSystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob(
        [](Data1* d1, Data2* d2) { cout << "writer_sys0" << endl; },
        "writer_sys0");
    schedule.RegisterEntityJob([](Data1* d) { cout << "writer_sys1" << endl; },
                               "writer_sys1");
    schedule.RegisterEntityJob([](Data2* d2) { cout << "writer_sys2" << endl; },
                               "writer_sys2");
    schedule.RegisterEntityJob(
        [](Data1* d, Data2* d2) { cout << "writer_sys3" << endl; },
        "writer_sys3");

    schedule.Order("writer_sys0", "writer_sys1")
        .Order("writer_sys1", "writer_sys3");
  }
};

int main() {
  World w;
  auto [dataSystem] = w.systemMngr.systemTraits.Register<DataSystem>();

  w.entityMngr.cmptTraits.Register<Data1, Data2>();

  w.entityMngr.Create<Data1, Data2>();

  w.systemMngr.Activate(dataSystem);
  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;
  cout << w.GenUpdateFrameGraph().Dump() << endl;

  return 0;
}
