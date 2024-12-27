//
// Created by Admin on 27/12/2024.
//

#pragma once

#include "../core/Entity.h"

#include <set>

namespace My::Cmpt {
// auto delete children
class Node {
 public:
  Node(My::Entity* entity) : entity(entity) {}

  ~Node();

  inline My::Entity* Entity() noexcept { return entity; }

  inline const My::Entity* Entity() const noexcept { return entity; }

  // don't call it in parallel
  void AddChild(Node* child);
  void DelChild(Node* child);

  bool IsDescendantOf(Node* e) const;

 private:
  Node(const Node& node) = delete;
  Node& operator=(const Node& node) = delete;

  My::Entity* entity;
  Node* parent{nullptr};
  std::set<Node*> children;
};
}  // namespace My::Cmpt