//
// Created by Admin on 12/03/2025.
//

#include <MyECS/SystemMngr.h>
 
 #include <MyECS/IListener.h>
 
 using namespace My;
 
void SystemMngr::Accept(IListener* listener) const {
  listener->EnterSystemMngr(this);
  for (const auto& [n, f] : onUpdateMap) {
    listener->EnterSystem(n);
    listener->ExistSystem(n);
  }
  listener->ExistSystemMngr(this);
}
