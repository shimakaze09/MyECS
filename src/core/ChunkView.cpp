//
// Created by Admin on 13/03/2025.
//

#include <MyECS/ChunkView.h>

#include <MyECS/detail/Archetype.h>

using namespace My::MyECS;

void* ChunkView::GetCmptArray(CmptType t) const {
  return archetype->Locate(chunkIdx, t);
}

size_t ChunkView::EntityNum() const noexcept {
  return archetype->EntityNumOfChunk(chunkIdx);
}

bool ChunkView::Contains(CmptType t) const {
  return archetype->GetCmptTypeSet().Contains(t);
}
