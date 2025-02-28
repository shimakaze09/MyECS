//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace std;
using namespace My;

struct Position {
  float x;

  void OnUpdate() const {
    // cout << "position: " << x << endl;
  }
};

struct Velocity {
  float x;

  void Update(Position* p) const {
    for (size_t i = 0; i < 1000000; i++)
      p->x *= x;
  }

  void Update() const {}

  static void OnUpdateSchedule(SystemSchedule& schedule) {
    schedule.Regist(MemFuncOf<void(Position*) const>::run(&Velocity::Update))
        .Regist(MemFuncOf<void() const>::run(&Velocity::Update));
  }
};

struct Acceleration {
  float x;

  void OnUpdate(Velocity* v) const {
    for (size_t i = 0; i < 1000000; i++)
      v->x *= x;
  }
};

int main() {
  CmptRegister::Instance().Regist<Position, Velocity, Acceleration>();

  World w;
  for (size_t i = 0; i < 10000; i++)
    w.CreateEntity<Velocity, Position, Acceleration>();
  w.Update();
  cout << w.DumpUpdateTaskflow() << endl;
  return 0;
}
