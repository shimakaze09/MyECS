//
// Created by Admin on 28/02/2025.
//

#pragma once

#include "CmptLifecycleMngr.h"
#include "SystemMngr.h"

#include <MyDP/Reflection/Reflection.h>

#include <MyTemplate/Concept.h>

namespace My::detail::CmptRegister_ {
template <typename T>
Concept(HaveOnRegist, &T::OnRegist);
}

namespace My {
template <typename... Cmpts>
void CmptRegister::Regist() {
  (RegistOne<Cmpts>(), ...);
}

template <typename... Cmpts>
bool CmptRegister::IsRegisted() const noexcept {
  return (IsRegistedOne<Cmpts>() && ...);
}

template <typename Cmpt>
bool CmptRegister::IsRegistedOne() const noexcept {
  return registedCmpts.find(TypeID<Cmpt>) != registedCmpts.end();
}

template <typename Cmpt>
void CmptRegister::RegistOne() {
  if (IsRegisted<Cmpt>())
    return;

  if constexpr (Require<detail::CmptRegister_::HaveOnRegist, Cmpt>)
    Cmpt::OnRegist();

  SystemMngr::Instance().Regist<Cmpt>();
  CmptLifecycleMngr::Instance().Regist<Cmpt>();
  Reflection<Cmpt>::Init();

  registedCmpts.insert(TypeID<Cmpt>);
}
}  // namespace My
