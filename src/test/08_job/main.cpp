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

class MySystem : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    auto buffer = std::make_shared<std::vector<size_t>>();
    auto f = schedule.Register(
        [buffer](size_t idxInQuery, const Data* data) {
          buffer->at(idxInQuery) = data->value;
        },
        "system function");
    schedule.Register(
        [buffer]() {
          size_t sum = 0;
          for (size_t i : *buffer)
            sum += i;
          cout << sum << endl;
        },
        "job");
    schedule.Order("system function", "job");

    size_t num = GetWorld()->entityMngr.EntityNum(f->entityQuery);
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
