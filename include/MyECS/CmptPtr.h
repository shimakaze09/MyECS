//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <cassert>

#include "CmptType.h"

namespace My::MyECS {
// CmptType + void*
class CmptPtr {
 public:
  CmptPtr(CmptType type, void* p) : type{type}, p{p} {}

  template <typename Cmpt>
  CmptPtr(Cmpt* p) : type{CmptType::Of<Cmpt>}, p{p} {}

  CmptType Type() const noexcept { return type; }

  void* Ptr() const noexcept { return p; }

  // for static Component
  template <typename Cmpt>
  Cmpt* As() const noexcept {
    assert(type.Is<Cmpt>());
    return reinterpret_cast<Cmpt*>(p);
  }

 private:
  CmptType type;
  void* p;
};

// CmptType + const void*
class CmptCPtr {
 public:
  CmptCPtr(CmptType type, const void* p) : type{type}, p{p} {}

  template <typename Cmpt>
  CmptCPtr(const Cmpt* p) : type{CmptType::Of<Cmpt>}, p{p} {}

  CmptType Type() const noexcept { return type; }

  const void* Ptr() const noexcept { return p; }

  // for static Component
  template <typename Cmpt>
  const Cmpt* As() const noexcept {
    assert(type.Is<Cmpt>());
    return reinterpret_cast<Cmpt*>(p);
  }

 private:
  CmptType type;
  const void* p;
};
}  // namespace My::MyECS
