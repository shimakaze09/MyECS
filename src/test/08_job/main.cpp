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
    auto buffer = std::make_shared<std::vector<size_t>>();
    schedule
        .Request(
            [buffer]() {
              for (size_t i = 0; i < 10; i++)
                buffer->push_back(i);
            },
            "job0")
        .Request(
            [buffer]() {
              for (size_t i : *buffer)
                cout << i << endl;
            },
            "job1")
        .Order("job0", "job1");
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
