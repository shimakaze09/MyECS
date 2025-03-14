//
// Created by Admin on 5/03/2025.
//

#pragma once

#include "CmptType.h"

#include <functional>
#include <unordered_map>

namespace My::MyECS {
// run-time dynamic component traits, singleton
// size (> 0) is necessary
// optional
// - alignment: alignof(std::max_align_t) as default, 8 / 16 in most cases
// - default constructor: do nothing as default
// - copy constructor: memcpy as default
// - move constructor: memcpy as default
// - move assignment: memcpy as default
// - destructor: do nothing as default
// - name
class RTDCmptTraits {
 public:
  static constexpr size_t default_alignment = alignof(std::max_align_t);

  static RTDCmptTraits& Instance() noexcept;

  RTDCmptTraits& Clear();

  // necessary
  RTDCmptTraits& RegisterSize(CmptType, size_t size);

  // optional
  RTDCmptTraits& RegisterAlignment(CmptType, size_t alignment);

  // optional
  RTDCmptTraits& RegisterDefaultConstructor(CmptType,
                                            std::function<void(void*)>);

  // optional
  RTDCmptTraits& RegisterCopyConstructor(CmptType,
                                         std::function<void(void*, void*)>);

  // optional
  RTDCmptTraits& RegisterMoveConstructor(CmptType,
                                         std::function<void(void*, void*)>);

  // optional
  RTDCmptTraits& RegisterMoveAssignment(CmptType,
                                        std::function<void(void*, void*)>);

  // optional
  RTDCmptTraits& RegisterDestructor(CmptType, std::function<void(void*)>);

  // optional
  RTDCmptTraits& RegisterName(CmptType, std::string name);

  size_t Sizeof(CmptType) const;
  size_t Alignof(CmptType) const;
  void CopyConstruct(CmptType, void* dst, void* src) const;
  void MoveConstruct(CmptType, void* dst, void* src) const;
  void MoveAssign(CmptType, void* dst, void* src) const;
  void Destruct(CmptType, void* cmpt) const;
  std::string_view Nameof(CmptType) const;

  RTDCmptTraits& Deregister(CmptType) noexcept;

  template <typename... Cmpts>
  void Register();

  template <typename Cmpt>
  void Deregister();

 private:
  friend class RTSCmptTraits;
  friend class Archetype;
  friend class EntityMngr;

  RTDCmptTraits() = default;

  // register all for Cmpt
  // static_assert
  // - is_default_constructible_v
  // - is_copy_constructible_v
  // - is_move_constructible_v
  // - is_destructible_v
  template <typename Cmpt>
  void RegisterOne();

  std::unordered_map<CmptType, size_t> sizeofs;
  std::unordered_map<CmptType, size_t> alignments;
  std::unordered_map<CmptType, std::function<void(void*)>>
      default_constructors;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*, void*)>>
      copy_constructors;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*, void*)>>
      move_constructors;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*, void*)>>
      move_assignments;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*)>> destructors;
  std::unordered_map<CmptType, std::string> names;
};
}  // namespace My::MyECS

#include "detail/RTDCmptTraits.inl"
