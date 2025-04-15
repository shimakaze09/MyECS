#pragma once

#include "AccessTypeID.h"

#include <functional>
#include <string>
#include <unordered_map>

namespace My::MyECS {
// run-time dynamic component traits
// size (> 0) is neccessary
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
  static constexpr std::size_t DefaultAlignment() noexcept {
    return alignof(std::max_align_t);
  }

  RTDCmptTraits& Clear();

  RTDCmptTraits& RegisterSize(TypeID, std::size_t size);
  RTDCmptTraits& RegisterAlignment(TypeID, std::size_t alignment);
  RTDCmptTraits& RegisterDefaultConstructor(TypeID, std::function<void(void*)>);
  RTDCmptTraits& RegisterCopyConstructor(TypeID,
                                         std::function<void(void*, void*)>);
  RTDCmptTraits& RegisterMoveConstructor(TypeID,
                                         std::function<void(void*, void*)>);
  RTDCmptTraits& RegisterMoveAssignment(TypeID,
                                        std::function<void(void*, void*)>);
  RTDCmptTraits& RegisterDestructor(TypeID, std::function<void(void*)>);
  RTDCmptTraits& RegisterName(TypeID, std::string name);

  const auto& GetSizeofs() const noexcept { return sizeofs; }

  const auto& GetAlignments() const noexcept { return alignments; };

  const auto& GetDefaultConstructors() const noexcept {
    return default_constructors;
  }

  const auto& GetCopyConstructors() const noexcept { return copy_constructors; }

  const auto& GetMoveConstructors() const noexcept { return move_constructors; }

  const auto& GetMoveAssignments() const noexcept { return move_assignments; }

  const auto& GetDestructors() const noexcept { return destructors; }

  const auto& GetNames() const noexcept { return names; }

  std::size_t Sizeof(TypeID) const;
  std::size_t Alignof(TypeID) const;
  void DefaultConstruct(TypeID, void* cmpt) const;
  void CopyConstruct(TypeID, void* dst, void* src) const;
  void MoveConstruct(TypeID, void* dst, void* src) const;
  void MoveAssign(TypeID, void* dst, void* src) const;
  void Destruct(TypeID, void* cmpt) const;
  std::string_view Nameof(TypeID) const;

  RTDCmptTraits& Deregister(TypeID) noexcept;

  template <typename... Cmpts>
  void Register();

  template <typename Cmpt>
  void Deregister();

 private:
  // register all for Cmpt
  // static_assert
  // - is_default_constructible_v
  // - is_copy_constructible_v || std::is_constructible_v<Cmpt, Cmpt&>
  // - is_move_constructible_v
  // - is_move_assignable_v
  // - is_destructible_v
  template <typename Cmpt>
  void RegisterOne();

  std::unordered_map<TypeID, std::size_t> sizeofs;
  std::unordered_map<TypeID, std::size_t> alignments;
  std::unordered_map<TypeID, std::function<void(void*)>>
      default_constructors;  // dst <- src
  std::unordered_map<TypeID, std::function<void(void*, void*)>>
      copy_constructors;  // dst <- src
  std::unordered_map<TypeID, std::function<void(void*, void*)>>
      move_constructors;  // dst <- src
  std::unordered_map<TypeID, std::function<void(void*, void*)>>
      move_assignments;  // dst <- src
  std::unordered_map<TypeID, std::function<void(void*)>> destructors;
  std::unordered_map<TypeID, std::string> names;
};
}  // namespace My::MyECS

#include "details/RTDCmptTraits.inl"
