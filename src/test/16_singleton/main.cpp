#include <MyECS/MyECS.hpp>
#include <iostream>

using namespace My::MyECS;

struct Timer {
  float dt{0.f};
};

struct Position {
  float val{1.f};
};
struct Velocity {
  float val{1.f};
};

struct MoverSystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterJob([](Singleton<Timer> timer) { timer->dt = 0.03f; },
                         "Timer");
    schedule.RegisterEntityJob(
        [](const Velocity* v, Position* p, Latest<Singleton<Timer>> timer) {
          p->val += timer->dt * v->val;
        },
        "Mover");
    schedule.RegisterEntityJob(
        [](const Position* p) { std::cout << p->val << std::endl; }, "Print");
  }
};

int main() {
  World w;
  w.entityMngr.cmptTraits.Register<Timer, Position, Velocity>();
  w.systemMngr.RegisterAndActivate<MoverSystem>();

  w.entityMngr.Create(My::TypeIDs_of<Position, Velocity>);
  w.entityMngr.Create(My::TypeIDs_of<Timer>);

  w.Update();
  std::cout << w.DumpUpdateJobGraph() << std::endl;
  std::cout << w.GenUpdateFrameGraph().Dump() << std::endl;
}
