//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Chunk.h"
#include "CmptLifecycleMngr.h"
#include "EntityBase.h"

#include <MyBL/Pool.h>

#include <MyTemplate/TypeID.h>
#include <MyTemplate/Typelist.h>

#include <map>
#include <set>

namespace My {
class ArchetypeMngr;
class Entity;

// TODO: clear up
class Archetype {
 public:
  // TODO: ID -> CmptIDSet
  struct ID : private std::set<size_t> {
    ID() = default;

    template <typename... Cmpts>
    ID(TypeList<Cmpts...>) noexcept {
      Add<Cmpts...>();
    }

    template <typename... Cmpts>
    void Add() noexcept {
      (insert(TypeID<Cmpts>), ...);
    }

    template <typename... Cmpts>
    void Remove() noexcept {
      (erase(TypeID<Cmpts>), ...);
    }

    template <typename... Cmpts>
    bool IsContain() const noexcept {
      return ((find(TypeID<Cmpts>) != end()) && ...);
    }

    bool IsContain(size_t cmptHash) const noexcept {
      return find(cmptHash) != end();
    }

    template <typename... Cmpts>
    bool Is() const noexcept {
      return sizeof...(Cmpts) == size() && IsContain<Cmpts...>();
    }

    using std::set<size_t>::begin;
    using std::set<size_t>::end;

    bool operator<(const ID& id) const noexcept;
    bool operator==(const ID& id) const noexcept;

    friend class Archetype;
  };

  // argument TypeList<Cmpts...> is for type deduction
  template <typename... Cmpts>
  Archetype(ArchetypeMngr* mngr, TypeList<Cmpts...>) noexcept;

  template <typename... Cmpts>
  static Archetype* Add(Archetype* from) noexcept;
  template <typename... Cmpts>
  static Archetype* Remove(Archetype* from) noexcept;

  ~Archetype();

  template <typename... Cmpts>
  const std::vector<std::tuple<Cmpts*...>> Locate();

  std::tuple<void*, size_t> At(size_t cmptHash, size_t idx);

  template <typename Cmpt>
  Cmpt* At(size_t idx);

  std::vector<std::tuple<void*, size_t>> Components(size_t idx);

  // no init
  size_t RequestBuffer() {
    if (num == chunks.size() * chunkCapacity)
      chunks.push_back(chunkPool.Request());
    return num++;
  }

  // init cmpts
  template <typename... Cmpts>
  const std::tuple<size_t, std::tuple<Cmpts*...>> CreateEntity();

  // erase idx-th empty entity
  // if idx != num-1, back entity will put at idx, return num-1
  // else return static_cast<size_t>(-1)
  size_t Erase(size_t idx);

  inline size_t Size() const noexcept { return num; }

  inline size_t ChunkNum() const noexcept { return chunks.size(); }

  inline size_t ChunkCapacity() const noexcept { return chunkCapacity; }

  inline const ID& GetID() const noexcept { return id; }

  inline ArchetypeMngr* GetArchetypeMngr() const noexcept { return mngr; }

  inline size_t CmptNum() const noexcept { return id.size(); }

  template <typename... Cmpts>
  inline bool IsContain() const noexcept;

 private:
  Archetype() = default;

  friend class Entity;
  friend class ArchetypeMngr;

  ArchetypeMngr* mngr;
  ID id;
  std::map<size_t, std::tuple<size_t, size_t>> h2so;  // hash to (size, offset)
  size_t chunkCapacity;
  std::vector<Chunk*> chunks;
  size_t num{0};

  Pool<Chunk> chunkPool;
};
}  // namespace My

#include "Archetype.inl"
