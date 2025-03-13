//
// Created by Admin on 13/03/2025.
//

#pragma once

#include "CmptType.h"

namespace My::MyECS {
class Archetype;
struct Chunk;

class ChunkView {
 public:
  ChunkView(Archetype* archetype, size_t chunkIdx, Chunk* chunk)
      : archetype{archetype}, chunkIdx{chunkIdx}, chunk{chunk} {}

  bool Contains(CmptType) const;

  // if not contain, return nullptr
  void* GetCmptArray(CmptType) const;

  template <typename Cmpt>
  Cmpt* GetCmptArray() const {
    return reinterpret_cast<Cmpt*>(GetCmptArray(CmptType::Of<Cmpt>));
  }

  size_t EntityNum() const;

 private:
  Archetype* archetype;
  size_t chunkIdx;
  Chunk* chunk;
};
}  // namespace My::MyECS
