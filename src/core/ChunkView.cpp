//
// Created by Admin on 13/03/2025.
//

#include <MyECS/ChunkView.h>

#include <MyECS/details/Archetype.h>

using namespace My::MyECS;

ChunkView::ChunkView(Archetype* archetype, std::size_t chunkIdx) noexcept
    : archetype{archetype},
      chunkIdx{chunkIdx},
      entityNum{archetype->EntityNumOfChunk(chunkIdx)} {}

void* ChunkView::GetCmptArray(TypeID t) const {
  return archetype->Locate(chunkIdx, t);
}

bool ChunkView::Contains(TypeID t) const {
  return archetype->GetTypeIDSet().Contains(t);
}
