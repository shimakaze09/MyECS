//
// Created by Admin on 13/03/2025.
//

#include <MyECS/ChunkView.h>

#include <MyECS/detail/Archetype.h>

using namespace My::MyECS;

ChunkView::ChunkView(Archetype* archetype, size_t chunkIdx) noexcept
    : archetype{archetype},
      chunkIdx{chunkIdx},
      entityNum{archetype->EntityNumOfChunk(chunkIdx)} {}

void* ChunkView::GetCmptArray(CmptType t) const {
  return archetype->Locate(chunkIdx, t);
}

bool ChunkView::Contains(CmptType t) const {
  return archetype->GetCmptTypeSet().Contains(t);
}
