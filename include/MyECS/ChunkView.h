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
  ChunkView() noexcept = default;

  bool Contains(CmptType) const;

  size_t EntityNum() const noexcept { return entityNum; }

  // nullptr if not contain
  void* GetCmptArray(CmptType) const;

  template <typename Cmpt>
  Span<Cmpt> GetCmptArray() const;
  Span<const Entity> GetEntityArray() const;

 private:
  Archetype* archetype{nullptr};
  size_t chunkIdx{static_cast<size_t>(-1)};
  size_t entityNum{0};
};
}  // namespace My::MyECS

#include "detail/ChunkView.inl"
