//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "detail/ArchetypeMngr.h"

namespace My {
class Entity final : private EntityBase {
 public:
  template <typename Cmpt>
  inline Cmpt* Get();

  template <typename Cmpt>
  inline const Cmpt* Get() const {
    return const_cast<Entity*>(this)->Get();
  }

  inline const std::vector<std::tuple<void*, size_t>> Components() const;

  template <typename... Cmpts>
  inline std::tuple<Cmpts*...> Attach();

  template <typename Cmpt>
  inline Cmpt* GetOrAttach();

  template <typename... Cmpts>
  inline void Detach();

  inline bool IsAlive() const noexcept;

  inline void Release() noexcept;
};

static_assert(sizeof(Entity) == sizeof(EntityBase) &&
              std::is_base_of_v<EntityBase, Entity>);
}  // namespace My

#include "detail/Entity.inl"