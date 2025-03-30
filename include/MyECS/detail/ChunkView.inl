//
// Created by Admin on 28/03/2025.
//

#pragma once

namespace My::MyECS {
template <typename Cmpt>
Span<Cmpt> ChunkView::GetCmptArray() const {
  auto* ptr = GetCmptArray(CmptType::Of<Cmpt>);
  if (!ptr)
    return {};
  return {static_cast<Cmpt*>(ptr), EntityNum()};
}

inline Span<const Entity> ChunkView::GetEntityArray() const {
  auto* ptr = GetCmptArray(CmptType::Of<Entity>);
  if (!ptr)
    return {};
  return {static_cast<const Entity*>(ptr), EntityNum()};
}
}  // namespace My::MyECS
