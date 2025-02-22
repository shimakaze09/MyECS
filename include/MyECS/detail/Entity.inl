//
// Created by Admin on 27/12/2024.
//

#pragma once

#include "SystemMngr.h"

namespace My {
template <typename Cmpt>
Cmpt* Entity::Get() {
  assert(IsAlive());
  return archetype->At<Cmpt>(idx);
}

template <typename Cmpt>
const Cmpt* Entity::Get() const {
  return const_cast<Entity*>(this)->Get();
}

template <typename... Cmpts>
std::tuple<Cmpts*...> Entity::Attach() {
  static_assert(sizeof...(Cmpts) > 0);
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Components must be different");
  (CmptMngr::Instance().Regist<Cmpts>(), ...);
  assert(IsAlive());
  (archetype->sysmngr->Regist<Cmpts>(), ...);
  return archetype->mngr->EntityAttach<Cmpts...>(this);
}

template <typename Cmpt>
inline Cmpt* Entity::GetOrAttach() {
  assert(IsAlive());
  Cmpt* cmpt = archetype->At<Cmpt>(idx);
  if (!cmpt)
    std::tie(cmpt) = Attach<Cmpt>();
  return cmpt;
}

template <typename... Cmpts>
void Entity::Detach() {
  static_assert(sizeof...(Cmpts) > 0);
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Components must be different");
  assert(IsAlive());
  return archetype->mngr->EntityDetach<Cmpts...>(this);
}
}  // namespace My