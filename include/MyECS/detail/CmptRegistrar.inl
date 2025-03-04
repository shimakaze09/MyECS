//
// Created by Admin on 28/02/2025.
//

#pragma once

#include "CmptLifecycleMngr.h"
#include "CmptSysMngr.h"

#include <MyTemplate/Concept.h>

namespace My::detail::CmptRegistrar_ {
template <typename T>
Concept(HaveOnRegister, &T::OnRegister);
}  // namespace My::detail::CmptRegistrar_

namespace My {
template <typename... Cmpts>
void CmptRegistrar::Register() {
  (RegisterOne<Cmpts>(), ...);
}

template <typename... Cmpts>
bool CmptRegistrar::IsRegistered() const noexcept {
  return (IsRegisteredOne<Cmpts>() && ...);
}

template <typename Cmpt>
bool CmptRegistrar::IsRegisteredOne() const noexcept {
  return registeredCmpts.find(TypeID<Cmpt>) != registeredCmpts.end();
}

template <typename Cmpt>
void CmptRegistrar::RegisterOne() {
  if (IsRegistered<Cmpt>())
    return;

  if constexpr (Require<detail::CmptRegistrar_::HaveOnRegister, Cmpt>)
    Cmpt::OnRegister();

  CmptSysMngr::Instance().Register<Cmpt>();
  CmptLifecycleMngr::Instance().Register<Cmpt>();

  registeredCmpts.insert(TypeID<Cmpt>);
}
}  // namespace My
