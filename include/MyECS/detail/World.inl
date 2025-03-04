//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "../CmptRegistrar.h"

#include <MyTemplate/Func.h>

namespace My {
template <typename... Cmpts>
std::tuple<EntityPtr, Cmpts*...> World::CreateEntity() {
  assert("World::CreateEntity: <Cmpts> are unregistered" &&
         CmptRegistrar::Instance().template IsRegistered<Cmpts...>());
  auto rst = entityMngr.CreateEntity<Cmpts...>();
  return {EntityPtr{reinterpret_cast<Entity*>(std::get<0>(rst))},
          std::get<1 + Find_v<TypeList<Cmpts...>, Cmpts>>(rst)...};
}
}  // namespace My
