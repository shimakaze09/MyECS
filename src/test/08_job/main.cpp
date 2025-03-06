//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

struct Data {
  size_t value;
};

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    auto buffer = std::make_shared<std::vector<size_t>>();
    schedule
        .Register(
            [buffer](size_t idxInQuery, const Data* data) {
              buffer->at(idxInQuery) = data->value;
            },
            "system function")
        .Register(
            [buffer]() {
              size_t sum = 0;
              for (size_t i : *buffer) sum += i;
              cout << sum << endl;
            },
            "job")
        .Order("system function", "job");
    size_t num = schedule.EntityNumInQuery("system function");
    buffer->resize(num);
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  for (size_t i = 1; i <= 100; i++) {
    auto [e] = w.entityMngr.Create();
    w.entityMngr.Emplace<Data>(e, i);
  }

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
