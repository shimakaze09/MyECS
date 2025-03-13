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

  // unchecked
  void* Ptr() const noexcept { return p; }

  // unchecked
  template <typename Cmpt>
  Cmpt* As() const noexcept {
    return reinterpret_cast<Cmpt*>(p);
  }

  template <typename Cmpt>
  LastFrame<Cmpt> AsLastFrame() const noexcept {
    assert(type.GetAccessMode() == AccessMode::LAST_FRAME);
    return p;
  }

  template <typename Cmpt>
  Write<Cmpt> AsWrite() const noexcept {
    assert(type.GetAccessMode() == AccessMode::WRITE);
    return p;
  }

  template <typename Cmpt>
  Latest<Cmpt> AsLatest() const noexcept {
    assert(type.GetAccessMode() == AccessMode::LATEST);
    return p;
  }

 private:
  CmptType type;
  void* p;
};
}  // namespace My::MyECS
