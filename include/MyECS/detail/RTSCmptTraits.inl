//
// Created by Admin on 5/03/2025.
//

#pragma once

#include "../RTDCmptTraits.h"

#include <stdexcept>

namespace My::MyECS {
inline size_t RTSCmptTraits::Sizeof(CmptType type) const {
  assert(sizeofs.find(type) != sizeofs.end());
  return sizeofs.find(type)->second;
}

inline size_t RTSCmptTraits::Alignof(CmptType type) const {
  assert(alignments.find(type) != alignments.end());
  return alignments.find(type)->second;
}

inline void RTSCmptTraits::CopyConstruct(CmptType type, void* dst,
                                         void* src) const {
  auto target = copy_constructors.find(type);

  if (target != copy_constructors.end())
    target->second(dst, src);
  else
    memcpy(dst, src, Sizeof(type));
}

inline void RTSCmptTraits::MoveConstruct(CmptType type, void* dst,
                                         void* src) const {
  auto target = move_constructors.find(type);

  if (target != move_constructors.end())
    target->second(dst, src);
  else
    memcpy(dst, src, Sizeof(type));
}

inline void RTSCmptTraits::MoveAssign(CmptType type, void* dst,
                                      void* src) const {
  auto target = move_assignments.find(type);

  if (target != move_assignments.end())
    target->second(dst, src);
  else
    memcpy(dst, src, Sizeof(type));
}

inline void RTSCmptTraits::Destruct(CmptType type, void* cmpt) const {
  auto target = destructors.find(type);
  if (target != destructors.end())
    target->second(cmpt);
}

template <typename Cmpt>
void RTSCmptTraits::Register() {
  static_assert(std::is_copy_constructible_v<Cmpt>,
                "<Cmpt> must be copy-constructible");
  static_assert(std::is_move_constructible_v<Cmpt>,
                "<Cmpt> must be move-constructible");
  static_assert(std::is_move_assignable_v<Cmpt>,
                "<Cmpt> must be move-assignable");
  static_assert(std::is_destructible_v<Cmpt>, "<Cmpt> must be destructible");

  constexpr CmptType type = CmptType::Of<Cmpt>;

  sizeofs[type] = sizeof(Cmpt);
  alignments[type] = alignof(Cmpt);

  if constexpr (!std::is_trivially_destructible_v<Cmpt>) {
    destructors[type] = [](void* cmpt) {
      reinterpret_cast<Cmpt*>(cmpt)->~Cmpt();
    };
  }
  if constexpr (!std::is_trivially_move_constructible_v<Cmpt>) {
    move_constructors[type] = [](void* dst, void* src) {
      new (dst) Cmpt(std::move(*reinterpret_cast<Cmpt*>(src)));
    };
  }
  if constexpr (!std::is_trivially_copy_assignable_v<Cmpt>) {
    move_assignments[type] = [](void* dst, void* src) {
      *reinterpret_cast<Cmpt*>(dst) = std::move(*reinterpret_cast<Cmpt*>(src));
    };
  }
  if constexpr (!std::is_trivially_copy_constructible_v<Cmpt>) {
    copy_constructors[type] = [](void* dst, void* src) {
      new (dst) Cmpt(*reinterpret_cast<Cmpt*>(src));
    };
  }
}

template <typename Cmpt>
void RTSCmptTraits::Deregister() {
  constexpr CmptType type = CmptType::Of<Cmpt>;

  sizeofs.erase(type);
  alignments.erase(type);

  if constexpr (!std::is_trivially_destructible_v<Cmpt>)
    destructors.erase(type);
  if constexpr (!std::is_trivially_copy_constructible_v<Cmpt>)
    copy_constructors.erase(type);
  if constexpr (!std::is_trivially_move_constructible_v<Cmpt>)
    move_constructors.erase(type);
  if constexpr (!std::is_trivially_move_assignable_v<Cmpt>)
    move_assignments.erase(type);
}

inline void RTSCmptTraits::Register(CmptType type) {
  const auto& rtdct = RTDCmptTraits().Instance();
  auto size_target = rtdct.sizeofs.find(type);
  if (size_target == rtdct.sizeofs.end())
    throw std::logic_error(
        "RTSCmptTraits::Register: RTDCmptTraits hasn't registered <CmptType>");
  sizeofs[type] = size_target->second;

  auto alignment_target = rtdct.alignments.find(type);
  if (alignment_target == rtdct.alignments.end())
    alignments[type] = RTDCmptTraits::default_alignment;
  else
    alignments[type] = alignment_target->second;

  auto destructor_target = rtdct.destructors.find(type);
  auto copy_constructor_target = rtdct.copy_constructors.find(type);
  auto move_constructor_target = rtdct.move_constructors.find(type);
  auto move_assignments_target = rtdct.move_assignments.find(type);

  if (destructor_target != rtdct.destructors.end())
    destructors[type] = destructor_target->second;
  if (copy_constructor_target != rtdct.copy_constructors.end())
    copy_constructors[type] = copy_constructor_target->second;
  if (move_constructor_target != rtdct.move_constructors.end())
    move_constructors[type] = move_constructor_target->second;
  if (move_assignments_target != rtdct.move_assignments.end())
    move_assignments[type] = move_assignments_target->second;
}

inline void RTSCmptTraits::Deregister(CmptType type) noexcept {
  sizeofs.erase(type);
  alignments.erase(type);
  copy_constructors.erase(type);
  move_constructors.erase(type);
  move_assignments.erase(type);
  destructors.erase(type);
}
}  // namespace My::MyECS
