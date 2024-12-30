//
// Created by Admin on 27/12/2024.
//

#pragma once

namespace My {
template <typename Cmpt>
inline Cmpt* Entity::Get() {
  assert(IsAlive());
  return archetype->At<Cmpt>(idx);
}

const std::vector<std::tuple<void*, size_t>> Entity::Components() const {
  return archetype->Components(idx);
}

template <typename... Cmpts>
inline std::tuple<Cmpts*...> Entity::Attach() {
  static_assert(sizeof...(Cmpts) > 0);
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Componnents must be different");
  static_assert(((std::is_constructible_v<Cmpts> ||
                  std::is_constructible_v<Cmpts, Entity*>) &&
                 ...));
  assert(IsAlive());
  return archetype->mngr->EntityAttach<Cmpts...>(this);
}

template <typename Cmpt>
inline Cmpt* Entity::GetOrAttach() {
  assert(IsAlive());
  Cmpt* cmpt = archetype->At<Cmpt>(idx);
  if (!cmpt)
    std::tie(cmpt) = Attach<Cmpt>();
  return cmpt;
}

template <typename... Cmpts>
inline void Entity::Detach() {
  static_assert(sizeof...(Cmpts) > 0);
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Componnents must be different");
  assert(IsAlive());
  return archetype->mngr->EntityDetach<Cmpts...>(this);
}

inline void Entity::Release() noexcept {
  assert(IsAlive());
  archetype->mngr->Release(this);
}

inline bool Entity::IsAlive() const noexcept {
  return archetype != nullptr;
}
}  // namespace My