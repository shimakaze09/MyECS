//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "../CmptPtr.h"
#include "../Entity.h"

#include "Chunk.h"
#include "CmptTypeSet.h"
#include "RTSCmptTraits.h"

#include <MyTemplate/TypeID.h>
#include <MyTemplate/Typelist.h>

#include <MyContainer/Pool.h>

#include <map>

namespace My {
class EntityMngr;

// Entity is a special Component
class Archetype {
 public:
  // argument TypeList<Cmpts...> is for type deduction
  // auto add Entity
  template <typename... Cmpts>
  Archetype(TypeList<Cmpts...>);

  // auto add Entity, use RTDCmptTraits
  template <typename... CmptTypes>
  static Archetype* New(CmptTypes...);

  // auto add Entity
  template <typename... Cmpts>
  static Archetype* Add(const Archetype* from);
  template <typename... CmptTypes>
  static Archetype* Add(const Archetype* from, CmptTypes...);
  // auto add Entity
  template <typename... Cmpts>
  static Archetype* Remove(const Archetype* from);
  template <typename... CmptTypes>
  static Archetype* Remove(const Archetype* from, CmptTypes...);

  ~Archetype();

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

  // init cmpts, set Entity
  // size_t: index in archetype
  template <typename... Cmpts>
  std::tuple<size_t, std::tuple<Cmpts*...>> CreateEntity(Entity e);
  size_t CreateEntity(Entity e);

  // return index in archetype
  size_t Instantiate(Entity e, size_t srcIdx);

  // erase idx-th entity
  // if idx != num-1, back entity will put at idx, return moved Entity's index
  // else return size_t_invalid
  size_t Erase(size_t idx);

  // Components + Entity
  const CmptTypeSet& GetCmptTypeSet() const noexcept { return types; }

  const RTSCmptTraits& GetRTSCmptTraits() const noexcept { return cmptTraits; }

  // no Entity
  size_t CmptNum() const noexcept { return types.size() - 1; }

  size_t EntityNum() const noexcept { return entityNum; }

  size_t ChunkNum() const noexcept { return chunks.size(); }

  size_t ChunkCapacity() const noexcept { return chunkCapacity; }

  template <typename... Cmpts>
  static constexpr size_t HashCode() noexcept {
    return CmptTypeSet::HashCodeOf<Entity, Cmpts...>();
  }

  template <typename... CmptTypes>
  static size_t HashCode(CmptTypes... types) noexcept {
    return CmptTypeSet{CmptType::Of<Entity>(), types...}.HashCode();
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
  RTSCmptTraits cmptTraits;
  std::unordered_map<CmptType, size_t>
      type2offset;  // CmptType to offset in chunk (include Entity)

  size_t chunkCapacity{static_cast<size_t>(-1)};
  std::vector<Chunk*> chunks;

  size_t entityNum{0};  // number of entities

  inline static Pool<Chunk> sharedChunkPool;  // no lock
};
}  // namespace My

#include "Archetype.inl"
