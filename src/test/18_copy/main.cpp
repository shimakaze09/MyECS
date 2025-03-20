//
// Created by Admin on 21/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;

struct Position { float val{ 0.f }; };
struct Velocity { float val{ 1.f }; };

struct MoverSystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob(
        [](const Velocity* v, Position* p) {
            p->val += v->val;
        },
        "Mover"
    );
    schedule.RegisterEntityJob(
        [](const Velocity* v, const Position* p) {
            std::cout << "v:" << v->val << std::endl;
            std::cout << "p:" << p->val << std::endl;
        },
        "Print"
    );
  }
};

int main() {
  World w;
  auto moverSystem = w.systemMngr.Register<MoverSystem>();
  w.systemMngr.Activate(moverSystem);
  w.entityMngr.Create<Position, Velocity>();

  World cpW = w;
	
  cpW.Update();
  cpW.entityMngr.Create<Position, Velocity>();
  cpW.Update();
}
