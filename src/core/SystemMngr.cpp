//
// Created by Admin on 12/03/2025.
//

#include <MyECS/SystemMngr.h>

#include <MyECS/IListener.h>

using namespace My::MyECS;

void SystemMngr::Accept(IListener* listener) const {
  listener->EnterSystemMngr(this);
  for (const auto& [n, system] : systems) {
    listener->EnterSystem(system.get());
    listener->ExistSystem(system.get());
  }
  listener->ExistSystemMngr(this);
}
