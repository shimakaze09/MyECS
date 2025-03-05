//
// Created by Admin on 26/12/2024.
//

#include <MyECS/World.h>

struct Position {
  float val;
};

struct Velocity {
  float val;
};

struct MoverSystem {
  static void OnUpdate(My::Schedule& schedule) {
    schedule.Request([](const Velocity* v, Position* p) { p->val += v->val; },
                     "MoverSystem");
  }
};

int main() {
  My::World w;
  w.systemMngr.Register<MoverSystem>();

  for (size_t i = 0; i < 10; i++)
    w.entityMngr.CreateEntity<>();

  w.Update();
}
