//
// Created by Admin on 26/12/2024.
//

#include <MyECS/World.h>

using namespace My::MyECS;

struct Position {
  float val;
};

struct Velocity {
  float val;
};

struct MoverSystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob(
        [](const Velocity* v, Position* p) { p->val += v->val; }, "Mover");
  }
};

int main() {
  World w;
  w.systemMngr.RegisterAndActivate<MoverSystem>();
  w.entityMngr.Create<Position, Velocity>();
  w.Update();
}
