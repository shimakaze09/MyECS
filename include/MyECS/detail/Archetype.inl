//
// Created by Admin on 26/12/2024.
//

#pragma once

#include <cassert>

namespace My {
template <typename... Cmpts>
Archetype::Archetype(TypeList<Cmpts...>) : types(TypeList<Entity, Cmpts...>{}) {
  static_assert(IsSet_v<TypeList<Entity, Cmpts...>>,
                "Archetype::Archetype: <Cmpts> must be different");
  cmptTraits.Register<Entity>();
  (cmptTraits.Register<Cmpts>(), ...);
  SetLayout();
}

template <typename... CmptTypes, typename>
static Archetype* Archetype::New(CmptTypes... types) {
  static_assert((std::is_same_v<CmptTypes, CmptType> && ...));
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr{types...};
  return New(typeArr.data(), typeArr.size());
}

template <typename... Cmpts>
Archetype* Archetype::Add(const Archetype* from) {
  assert(((!from->types.Contains(CmptType::Of<Cmpts>)) && ...));

  Archetype* rst = new Archetype;

  rst->types = from->types;
  rst->types.Insert(CmptType::Of<Cmpts>...);
  rst->cmptTraits = from->cmptTraits;
  (rst->cmptTraits.Register<Cmpts>(), ...);

  rst->SetLayout();

  return rst;
}

template <typename... CmptTypes, typename>
Archetype* Archetype::Add(const Archetype* from, CmptTypes... types) {
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr{types...};
  return Add(typeArr.data(), typeArr.size());
}

template <typename... CmptTypes, typename>
Archetype* Archetype::Remove(const Archetype* from, CmptTypes... types) {
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr{types...};
  return Remove(typeArr.data(), typeArr.size());
}

template <typename Cmpt>
Cmpt* Archetype::At(size_t idx) const {
  return reinterpret_cast<Cmpt*>(At(CmptType::Of<Cmpt>, idx));
}

template <typename... Cmpts>
std::tuple<size_t, std::tuple<Cmpts*...>> Archetype::Create(Entity e) {
  assert((types.Contains(CmptType::Of<Cmpts>) && ...) &&
         types.size() == 1 + sizeof...(Cmpts));
  static_assert((std::is_constructible_v<Cmpts> && ...),
                "Archetype::Create: <Cmpts> isn't constructible");
  static_assert(IsSet_v<TypeList<Entity, Cmpts...>>,
                "Archetype::Create: <Cmpts> must be different");

  size_t idx = RequestBuffer();
  size_t idxInChunk = idx % chunkCapacity;
  byte* buffer = chunks[idx / chunkCapacity]->Data();

  new (buffer + Offsetof(CmptType::Of<Entity>) + idxInChunk * sizeof(Entity))
      Entity(e);

  std::tuple<Cmpts*...> cmpts = {new (buffer + Offsetof(CmptType::Of<Cmpts>) +
                                      idxInChunk * sizeof(Cmpts)) Cmpts...};

  return {idx, cmpts};
}
}  // namespace My
