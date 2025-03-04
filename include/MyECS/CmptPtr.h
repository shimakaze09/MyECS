//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <MyDP/Basic/Read.h>
#include <MyTemplate/TypeID.h>

#include <cassert>

namespace My {
struct CmptPtr {
  Read<CmptPtr, size_t> id;

  CmptPtr(size_t id, void* p) : id{id}, p{p} {}

  template <typename Cmpt>
  CmptPtr(Cmpt* p) : id{TypeID<Cmpt>}, p{p} {}

  template <typename Cmpt>
  Cmpt* As() const noexcept {
    assert(id == TypeID<Cmpt>);
    return reinterpret_cast<Cmpt*>(p);
  }

  template <typename Cmpt>
  const Cmpt* As() const noexcept {
    return const_cast<CmptPtr*>(this)->As<Cmpt>();
  }

  void* Ptr() noexcept { return p; }

  const void* Ptr() const noexcept { return p; }

 private:
  void* p;
};
}  // namespace My
