//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

struct RTDSystem {
  static void OnUpdate(Schedule& schedule) {
    EntityLocator locator(
        { CmptType{ "LuaCmpt" } }, // read: last frame
        {}, // write
        {} // read: latest
    );
    schedule.Request([](const EntityLocator* locator, void** cmpts) {
        for (auto type : locator->CmptTypes())
          cout << type.HashCode() << endl;
      }, "run-time dynamic func", locator);
  }
};

int main() {
  CmptType type("LuaCmpt");
  RTDCmptTraits::Instance().RegisterSize(type, 16);
  RTDCmptTraits::Instance().RegisterDefaultConstructor(type, [](void*) { cout << "construct" << endl;});
  RTDCmptTraits::Instance().RegisterDestructor(type, [](void*) { cout << "destructor" << endl; });

  World w;
  auto [e] = w.entityMngr.CreateEntity();
  w.entityMngr.Attach(e, type);

  w.systemMngr.Register<RTDSystem>();
  w.Update();
  cout << w.DumpUpdateJobGraph() << endl;

  return 0;
}
