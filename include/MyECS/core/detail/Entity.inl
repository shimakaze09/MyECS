namespace My {
template <typename Cmpt>
inline Cmpt* Entity::Get() {
  assert(IsAlive());
  return archetype->At<Cmpt>(idx);
}

template <typename... Cmpts>
inline std::tuple<Cmpts*...> Entity::Attach() {
  static_assert(sizeof...(Cmpts) > 0);
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Components must be different");
  static_assert(((std::is_constructible_v<Cmpts> ||
                  std::is_constructible_v<Cmpts, Entity*>) &&
                 ...));
  assert(IsAlive());
  return archetype->m_manager->EntityAttach<Cmpts...>(this);
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
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Components must be different");
  assert(IsAlive());
  return archetype->m_manager->EntityDetach<Cmpts...>(this);
}

inline void Entity::Release() noexcept {
  assert(IsAlive());
  archetype->m_manager->Release(this);
}

inline bool Entity::IsAlive() const noexcept {
  return archetype != nullptr;
}
}  // namespace My