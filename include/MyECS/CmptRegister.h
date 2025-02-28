//
// Created by Admin on 28/02/2025.
//

#pragma once

#include <unordered_set>

namespace My {
class CmptRegister {
 public:
  static CmptRegister& Instance() noexcept {
    static CmptRegister instance;
    return instance;
  }

  template <typename... Cmpts>
  void Regist();

  template <typename... Cmpts>
  bool IsRegisted() const noexcept;

 private:
  template <typename Cmpt>
  void RegistOne();

  template <typename Cmpt>
  bool IsRegistedOne() const noexcept;

  std::unordered_set<size_t> registedCmpts;
};
}  // namespace My

#include "detail/CmptRegister.inl"
