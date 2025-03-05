//
// Created by Admin on 5/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My;
using namespace std;

int main() {
  CmptType type("LuaCmpt A");
  RTDCmptTraits::Instance().RegisterSize(type, 16);

  World w;
  w.entityMngr.CreateEntity(type);
  return 0;
}
