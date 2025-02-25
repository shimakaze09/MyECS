//
// Created by Admin on 26/12/2024.
//

#pragma once

namespace My {
template <typename Sys>
void World::Each(Sys&& s) {
  detail::World_::Each<typename FuncTraits<Sys>::ArgList>::run(
      this, std::forward<Sys>(s));
}

template <typename Sys>
void World::Each(Sys&& s) const {
  detail::World_::Each<typename FuncTraits<Sys>::ArgList>::run(
      this, std::forward<Sys>(s));
}

template <typename Sys>
void World::ParallelEach(Sys&& s) {
  detail::World_::ParallelEach<typename FuncTraits<Sys>::ArgList>::run(
      this, std::forward<Sys>(s));
}

template <typename Sys>
void World::ParallelEach(Sys&& s) const {
  detail::World_::ParallelEach<typename FuncTraits<Sys>::ArgList>::run(
      this, std::forward<Sys>(s));
}

template <typename... Cmpts>
std::tuple<Entity*, Cmpts*...> World::CreateEntity() {
  static_assert(IsSet_v<TypeList<Cmpts...>>, "Componnents must be different");
  (CmptMngr::Instance().Regist<Cmpts>(), ...);
  auto rst = mngr.CreateEntity<Cmpts...>();
  (sysMngr.Regist<Cmpts>(), ...);
  return {reinterpret_cast<Entity*>(std::get<0>(rst)),
          std::get<1 + Find_v<TypeList<Cmpts...>, Cmpts>>(rst)...};
}
}  // namespace My

namespace My::detail::World_ {
template <typename... Cmpts>
struct Each<TypeList<Cmpts*...>> {
  static_assert(sizeof...(Cmpts) > 0);
  using CmptList = TypeList<Cmpts...>;
  static_assert(IsSet_v<CmptList>, "Componnents must be different");

  template <typename Sys>
  static void run(World* w, Sys&& s) {
    for (auto archetype : w->mngr.GetArchetypeWith<std::decay_t<Cmpts>...>()) {
      auto cmptsVecTuple = archetype->Locate<std::decay_t<Cmpts>...>();
      size_t num = archetype->Size();
      size_t chunkNum = archetype->ChunkNum();
      size_t chunkCapacity = archetype->ChunkCapacity();

      for (size_t i = 0; i < chunkNum; i++) {
        auto cmptsTuple = std::make_tuple(
            std::get<Find_v<CmptList, Cmpts>>(cmptsVecTuple)[i]...);
        size_t J = std::min(chunkCapacity, num - (i * chunkCapacity));
        for (size_t j = 0; j < J; j++)
          s((std::get<Find_v<CmptList, Cmpts>>(cmptsTuple) + j)...);
      }
    }
  }

  template <typename Sys>
  static void run(const World* w, Sys&& s) {
    static_assert((std::is_const_v<Cmpts> && ...),
                  "arguments must be const <Component>*");
    run(const_cast<World*>(w), std::forward<Sys>(s));
  }
};

template <typename... Cmpts>
struct ParallelEach<TypeList<Cmpts*...>> {
  static_assert(sizeof...(Cmpts) > 0);
  using CmptList = TypeList<Cmpts...>;
  static_assert(IsSet_v<CmptList>, "Componnents must be different");

  template <typename Sys>
  static void run(World* w, Sys&& s) {
    tf::Taskflow taskflow;
    w->mngr.GenTaskflow(&taskflow, std::forward<Sys>(s));
    w->executor.run(taskflow).wait();
  }

  template <typename Sys>
  static void run(const World* w, Sys&& s) {
    static_assert((std::is_const_v<Cmpts> && ...),
                  "arguments must be const <Component>*");
    run(const_cast<World*>(w), std::forward<Sys>(s));
  }
};
}  // namespace My::detail::World_
