//
// Created by Admin on 13/03/2025.
//

#include <MyECS/SystemFunc.h>

using namespace My::MyECS;

void SystemFunc::operator()(Entity e, size_t entityIndexInQuery,
                            RTDCmptsView rtdcmpts) {
  assert(mode == Mode::Entity);
  return func(e, entityIndexInQuery, rtdcmpts,
              ChunkView{nullptr, size_t_invalid, nullptr});
}

void SystemFunc::operator()(ChunkView chunkView) {
  assert(mode == Mode::Chunk);
  return func(Entity::Invalid(), size_t_invalid, RTDCmptsView{nullptr, nullptr},
              chunkView);
}

void SystemFunc::operator()() {
  assert(mode == Mode::Job);
  return func(Entity::Invalid(), size_t_invalid, RTDCmptsView{nullptr, nullptr},
              ChunkView{nullptr, size_t_invalid, nullptr});
}
