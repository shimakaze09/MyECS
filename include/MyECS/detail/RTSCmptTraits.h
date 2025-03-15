//
// Created by Admin on 30/12/2024.
//

#pragma once

#include "../CmptType.h"

#include <functional>
#include <unordered_map>

namespace My::MyECS {
// run-time static component traits
class RTSCmptTraits {
 public:
  size_t Sizeof(CmptType type) const;
  size_t Alignof(CmptType type) const;
  void CopyConstruct(CmptType type, void* dst, void* src) const;
  void MoveConstruct(CmptType type, void* dst, void* src) const;
  void MoveAssign(CmptType type, void* dst, void* src) const;
  void Destruct(CmptType type, void* cmpt) const;

  template <typename Cmpt>
  void Register();

  // use RTDCmptTraits
  void Register(CmptType type);

  template <typename Cmpt>
  void Deregister();
  void Deregister(CmptType type) noexcept;

 private:
  friend class Archetype;

  std::unordered_map<CmptType, size_t> sizeofs;
  std::unordered_map<CmptType, size_t> alignments;
  std::unordered_map<CmptType, std::function<void(void*, void*)>>
      copy_constructors;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*, void*)>>
      move_constructors;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*, void*)>>
      move_assignments;  // dst <- src
  std::unordered_map<CmptType, std::function<void(void*)>> destructors;
};
}  // namespace My::MyECS

#include "RTSCmptTraits.inl"
