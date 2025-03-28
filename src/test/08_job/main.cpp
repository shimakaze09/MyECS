//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;
using namespace std;

struct Data {
  size_t value;
};

struct MySystem {
  static void OnUpdate(Schedule& schedule) {
    auto buffer = std::make_shared<std::vector<size_t>>();
    auto f = schedule.RegisterEntityJob(
        [buffer](size_t idxInQuery, const Data* data) {
          buffer->at(idxInQuery) = data->value;
        },
        "use");
    schedule.RegisterJob(
        [buffer]() {
          size_t sum = 0;
          for (size_t i : *buffer)
            sum += i;
          cout << sum << endl;
        },
        "print");
    schedule.RegisterJob(
        [buffer, f](World* w) {
          size_t num = w->entityMngr.EntityNum(f->entityQuery);
          buffer->resize(num);
        },
        "allocate");
    schedule.Order("allocate", "use");
    schedule.Order("use", "print");
  }
};

int main() {
  World w;
  auto [mySystem] = w.systemMngr.systemTraits.Register<MySystem>();

  for (size_t i = 1; i <= 100; i++) {
    auto [e] = w.entityMngr.Create();
    w.entityMngr.Emplace<Data>(e, i);
  }

  w.systemMngr.Activate(mySystem);
  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
