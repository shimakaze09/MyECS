//
// Created by Admin on 26/12/2024.
//

#pragma once

#include <MyTemplate/TypeID.h>
#include <MyTemplate/Typelist.h>

#include <map>

#include "../CmptLocator.h"
#include "../CmptPtr.h"
#include "../Entity.h"
#include "Chunk.h"
#include "CmptTypeSet.h"
#include "RTSCmptTraits.h"

namespace My::MyECS {
class EntityMngr;

// Entity is a special Component
// type of Entity + Components is Archetype's type
class Archetype {
 public:
  // argument TypeList<Cmpts...> is for type deduction
  // auto add Entity
  template <typename... Cmpts>
  Archetype(EntityMngr*, TypeList<Cmpts...>);

  // copy
  Archetype(EntityMngr*, const Archetype&);

  ~Archetype();

  // auto add Entity, use RTDCmptTraits
  static Archetype* New(EntityMngr*, const CmptType* types, size_t num);

  // auto add Entity
  template <typename... Cmpts>
  static Archetype* Add(const Archetype* from);
  static Archetype* Add(const Archetype* from, const CmptType* types,
                        size_t num);

  // auto add Entity
  static Archetype* Remove(const Archetype* from, const CmptType* types,
                           size_t num);

  // Entity + Components
  std::tuple<std::vector<Entity*>, std::vector<std::vector<CmptAccessPtr>>,
             std::vector<size_t>>
  Locate(const CmptLocator& locator) const;

  // nullptr if not contains
  void* Locate(size_t chunkIdx, CmptType) const;

  Chunk* GetChunk(size_t chunkIdx) const { return chunks[chunkIdx]; }

  // nullptr if not contains
  void* At(CmptType, size_t idx) const;

  // nullptr if not contains
  template <typename Cmpt>
  Cmpt* At(size_t idx) const {
    return reinterpret_cast<Cmpt*>(At(CmptType::Of<Cmpt>, idx));
  }

  // no Entity
  std::vector<CmptPtr> Components(size_t idx) const;

  // no init
  size_t RequestBuffer();

  // init cmpts, set Entity
  // size_t: index in archetype
  template <typename... Cmpts>
  std::tuple<size_t, std::tuple<Cmpts*...>> Create(Entity);

  // use RTDCmptTraits's default constructor
  size_t Create(Entity);

  // return index in archetype
  size_t Instantiate(Entity, size_t srcIdx);

  // erase idx-th entity
  // if idx != num-1, back entity will put at idx, return moved Entity's index
  // else return size_t_invalid
  // move-assignment + destructor
  size_t Erase(size_t idx);

  // Components + Entity
  const CmptTypeSet& GetCmptTypeSet() const noexcept { return types; }

  const RTSCmptTraits& GetRTSCmptTraits() const noexcept { return cmptTraits; }

  size_t EntityNum() const noexcept { return entityNum; }

  size_t EntityNumOfChunk(size_t chunkIdx) const noexcept;

  size_t ChunkNum() const noexcept { return chunks.size(); }

  size_t ChunkCapacity() const noexcept { return chunkCapacity; }

  // add Entity
  static CmptTypeSet GenCmptTypeSet(const CmptType* types, size_t num);
  template <typename... Cmpts>
  static CmptTypeSet GenCmptTypeSet();

 private:
  Archetype(EntityMngr* entityMngr) : entityMngr{entityMngr} {}

  // set type2alignment
  // call after setting type2size and type2offset
  void SetLayout();

  size_t Offsetof(CmptType type) const {
    return type2offset.find(type)->second;
  }

  static bool NotContainEntity(const CmptType* types, size_t num) noexcept;

  friend class EntityMngr;
  EntityMngr* entityMngr;

  CmptTypeSet types;  // Entity + Components
  RTSCmptTraits cmptTraits;
  std::unordered_map<CmptType, size_t>
      type2offset;  // CmptType to offset in chunk (include Entity)

  size_t chunkCapacity{size_t_invalid};
  std::vector<Chunk*> chunks;

  size_t entityNum{0};  // number of entities
};
}  // namespace My::MyECS

#include "Archetype.inl"
