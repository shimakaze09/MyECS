#include <MyECS/MyECS.hpp>
#include <iostream>

using namespace Smkz::MyECS;

struct A {};
struct B {};
struct C {};

struct PrintASystem {
  static void OnUpdate(Schedule& schedule) {
    schedule.RegisterEntityJob([](A*) { std::cout << "A" << std::endl; },
                               "Parallel Print A");
    auto spilt = schedule.RegisterJob(
        []() { std::cout << "vvvvvvvvvv" << std::endl; }, "Spilt");
    schedule.RegisterEntityJob([](const A*) { std::cout << "A" << std::endl; },
                               "Serial Print A", false);
    schedule.Order("Parallel Print A", "Spilt");
    schedule.Order("Spilt", "Serial Print A");
  }
};

int main() {
  World w;
  w.entityMngr.cmptTraits.Register<A, B, C>();
  w.systemMngr.RegisterAndActivate<PrintASystem>();
  w.entityMngr.Create(Smkz::TypeIDs_of<A>);
  w.entityMngr.Create(Smkz::TypeIDs_of<A, B>);
  w.entityMngr.Create(Smkz::TypeIDs_of<A, C>);
  w.entityMngr.Create(Smkz::TypeIDs_of<A, B, C>);

  for (std::size_t i = 0; i < 5; i++) {
    w.Update();
    std::cout << "^^^^^^^^^^" << std::endl;
  }

  for (std::size_t i = 0; i < 100; i++) w.entityMngr.Create();

  w.RunEntityJob([](Entity e) { std::cout << e.index << std::endl; }, false);

  std::cout << w.DumpUpdateJobGraph() << std::endl;
  std::cout << w.GenUpdateFrameGraph().Dump() << std::endl;
}
