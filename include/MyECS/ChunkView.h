//
// Created by Admin on 13/03/2025.
//

#pragma once

#include "CmptType.h"

#include <MyContainer/Span.h>

namespace My::MyECS {
class Archetype;

class ChunkView {
 public:
  ChunkView(Archetype* archetype, size_t chunkIdx) noexcept;

  bool Contains(CmptType) const;

  size_t EntityNum() const noexcept { return entityNum; }

  // nullptr if not contain
  void* GetCmptArray(CmptType) const;

  template <typename Cmpt>
  Span<Cmpt> GetCmptArray() const {
    return {static_cast<Cmpt*>(GetCmptArray(CmptType::Of<Cmpt>)), EntityNum()};
  }

  Span<const Entity> GetEntityArray() const {
    return {static_cast<Entity*>(GetCmptArray(CmptType::Of<Entity>)),
            EntityNum()};
  }

 private:
  Archetype* archetype;
  size_t chunkIdx;
  size_t entityNum;
};
}  // namespace My::MyECS
