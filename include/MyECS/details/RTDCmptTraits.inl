#pragma once

#include "../RTDCmptTraits.hpp"

namespace My::MyECS {
template <typename... Cmpts>
void RTDCmptTraits::Register() {
  (RegisterOne<Cmpts>(), ...);
}

template <typename Cmpt>
void RTDCmptTraits::RegisterOne() {
  static_assert(!IsTaggedCmpt_v<Cmpt>, "<Cmpt> should not be tagged");
  static_assert(std::is_default_constructible_v<Cmpt>,
                "<Cmpt> must be default-constructible");
  static_assert(
      std::is_copy_constructible_v<Cmpt> ||
          std::is_constructible_v<Cmpt, Cmpt&>,
      "<Cmpt> must be copy-constructible or constructible with <Cmpt&>");
  static_assert(std::is_move_constructible_v<Cmpt>,
                "<Cmpt> must be move-constructible");
  static_assert(std::is_move_assignable_v<Cmpt>,
                "<Cmpt> must be move-assignable");
  static_assert(std::is_destructible_v<Cmpt>, "<Cmpt> must be destructible");

  constexpr Type type = Type_of<Cmpt>;

  if constexpr (std::is_trivial_v<Cmpt>) trivials.insert(TypeID_of<Cmpt>);

  sizeofs.emplace(type.GetID(), sizeof(Cmpt));
  alignments.emplace(type.GetID(), alignof(Cmpt));
  names.emplace(type.GetID(), type.GetName());

  default_constructors.emplace(type.GetID(),
                               [](void* cmpt) { new (cmpt) Cmpt{}; });
  if constexpr (!std::is_trivially_destructible_v<Cmpt>) {
    destructors.emplace(type.GetID(),
                        [](void* cmpt) { static_cast<Cmpt*>(cmpt)->~Cmpt(); });
  }
  if constexpr (!std::is_trivially_move_constructible_v<Cmpt>) {
    move_constructors.emplace(type.GetID(), [](void* dst, void* src) {
      new (dst) Cmpt(std::move(*static_cast<Cmpt*>(src)));
    });
  }
  if constexpr (!std::is_trivially_move_assignable_v<Cmpt>) {
    move_assignments.emplace(type.GetID(), [](void* dst, void* src) {
      *static_cast<Cmpt*>(dst) = std::move(*static_cast<Cmpt*>(src));
    });
  }
  if constexpr (!std::is_trivially_copy_constructible_v<Cmpt>) {
    copy_constructors.emplace(type.GetID(), [](void* dst, void* src) {
      new (dst) Cmpt(*static_cast<Cmpt*>(src));
    });
  }
}

template <typename Cmpt>
void RTDCmptTraits::Deregister() {
  constexpr TypeID type = TypeID_of<Cmpt>;

  sizeofs.erase(type);
  alignments.erase(type);
  names.erase(type);

  if constexpr (std::is_trivial_v<Cmpt>) trivials.erase(TypeID_of<Cmpt>);

  default_constructors.erase(type);
  if constexpr (!std::is_trivially_destructible_v<Cmpt>)
    destructors.erase(type);
  if constexpr (!std::is_trivially_move_constructible_v<Cmpt>)
    move_constructors.erase(type);
  if constexpr (!std::is_trivially_move_assignable_v<Cmpt>)
    move_assignments.erase(type);
  if constexpr (!std::is_trivially_copy_constructible_v<Cmpt>)
    copy_constructors.erase(type);
}
}  // namespace My::MyECS
