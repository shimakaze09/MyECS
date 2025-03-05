//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "../Entity.h"

#include "../CmptPtr.h"

#include "Chunk.h"
#include "CmptTypeSet.h"

#include "RuntimeCmptTraits.h"

#include <MyTemplate/TypeID.h>
#include <MyTemplate/Typelist.h>

#include <MyContainer/Pool.h>

#include <map>

namespace My {
class EntityMngr;

// Entity is a special Component
class Archetype {
 public:
  static constexpr size_t npos = static_cast<size_t>(-1);

  // argument TypeList<Cmpts...> is for type deduction
  // auto add Entity
  template <typename... Cmpts>
  Archetype(TypeList<Cmpts...>) noexcept;

  // auto add Entity
  template <typename... Cmpts>
  static Archetype* Add(Archetype* from) noexcept;
  // auto add Entity
  template <typename... Cmpts>
  static Archetype* Remove(Archetype* from) noexcept;

  ~Archetype();

  template <typename... Cmpts>
  const std::vector<std::tuple<Entity*, Cmpts*...>> Locate() const;

  // Entity + Components
  std::tuple<std::vector<Entity*>, std::vector<std::vector<void*>>,
             std::vector<size_t>>
  Locate(const std::set<CmptType>& cmptTypes) const;

  std::tuple<void*, size_t> At(CmptType type, size_t idx) const;

  template <typename Cmpt>
  Cmpt* At(size_t idx) const;

  // no Entity
  std::vector<CmptPtr> Components(size_t idx) const;

  // no init
  size_t RequestBuffer();

  // init cmpts
  // set Entity
  template <typename... Cmpts>
  const std::tuple<size_t, std::tuple<Cmpts*...>> CreateEntity(Entity e);

  // erase idx-th entity
  // if idx != num-1, back entity will put at idx, return moved Entity's index
  // else return Entity::npos
  size_t Erase(size_t idx);

  // Components + Entity
  const CmptTypeSet& GetCmptTypeSet() const noexcept { return types; }

  const RuntimeCmptTraits& GetRuntimeCmptTraits() const noexcept {
    return cmptTraits;
  }

  // no Entity
  size_t CmptNum() const noexcept { return types.size() - 1; }

  size_t EntityNum() const noexcept { return entityNum; }

  size_t ChunkNum() const noexcept { return chunks.size(); }

  size_t ChunkCapacity() const noexcept { return chunkCapacity; }

  template <typename... Cmpts>
  static constexpr size_t HashCode() noexcept {
    return CmptTypeSet::HashCodeOf<Entity, Cmpts...>();
  }

 private:
  Archetype() = default;

  // set type2alignment
  // call after setting type2size and type2offset
  void SetLayout();

  size_t Offsetof(CmptType type) const {
    return type2offset.find(type)->second;
  }

  friend class EntityMngr;

  CmptTypeSet types;  // Entity + Components
  RuntimeCmptTraits cmptTraits;
  std::map<CmptType, size_t>
      type2offset;  // CmptType to offset in chunk (include Entity)

  size_t chunkCapacity{static_cast<size_t>(-1)};
  std::vector<Chunk*> chunks;

  size_t entityNum{0};  // number of entities

  inline static Pool<Chunk> sharedChunkPool;  // no lock
};
}  // namespace My

#include "Archetype.inl"
