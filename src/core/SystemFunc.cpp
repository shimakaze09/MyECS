//
// Created by Admin on 13/03/2025.
//

#include <MyECS/SystemFunc.h>

using namespace My::MyECS;

void SystemFunc::operator()(World* w, Entity e, size_t entityIndexInQuery,
                            CmptsView rtdcmpts) {
  assert(mode == Mode::Entity);
  return func(w, e, entityIndexInQuery, rtdcmpts,
              ChunkView{nullptr, size_t_invalid, nullptr});
}

void SystemFunc::operator()(World* w, ChunkView chunkView) {
  assert(mode == Mode::Chunk);
  return func(w, Entity::Invalid(), size_t_invalid, CmptsView{nullptr, nullptr},
              chunkView);
}

void SystemFunc::operator()(World* w) {
  assert(mode == Mode::Job);
  return func(w, Entity::Invalid(), size_t_invalid, CmptsView{nullptr, nullptr},
              ChunkView{nullptr, size_t_invalid, nullptr});
}
