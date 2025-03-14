//
// Created by Admin on 14/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;

struct Timer {
  float dt;
};

struct Position {
  float val{1.f};
};

struct Velocity {
  float val{1.f};
};

class MoverSystem : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    float dt = GetWorld()->entityMngr.GetSingleton<Timer>()->dt;
    schedule.Register(
        [dt](const Velocity* v, Position* p) { p->val += dt * v->val; },
        "Mover");
    schedule.Register(
        [dt](const Position* p) { std::cout << p->val << std::endl; }, "Print");
  }
};

int main() {
  World w;
  w.systemMngr.Register<MoverSystem>();
  w.entityMngr.Create<Position, Velocity>();
  auto [e_singleton, timer] = w.entityMngr.Create<Timer>();
  timer->dt = 0.03f;

  w.Update();
}
