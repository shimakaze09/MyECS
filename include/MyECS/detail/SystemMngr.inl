//
// Created by Admin on 22/02/2025.
//

#pragma once

#include <nameof.hpp>

namespace My::MyECS {
template <typename System>
size_t SystemMngr::GetIndex() const {
  return GetIndex(nameof::nameof_type<System>());
}

template <typename... Systems>
std::array<size_t, sizeof...(Systems)> SystemMngr::Register() {
  return {Register(std::string{nameof::nameof_type<Systems>()},
                   &Systems::OnUpdate)...};
}

template <typename System>
void SystemMngr::Unregister() {
  Unregister(nameof::nameof_type<System>());
}
}  // namespace My::MyECS
