//
// Created by Admin on 27/12/2024.
//

#include <MyECS/cmpt/Node.h>

using namespace My::Cmpt;

Node::~Node() {
  std::cout << "Node release" << std::endl;
  for (auto child : children)
    child->entity->Release();

  if (parent) {
    parent->children.erase(this);
    parent = nullptr;
  }

  entity = nullptr;
}

void Node::AddChild(Node* child) {
  assert(child != this);
  if (child->parent)
    child->parent->DelChild(child);

  child->parent = this;
  children.insert(child);
}

void Node::DelChild(Node* child) {
  assert(child->parent == this);
  child->parent = nullptr;
  children.erase(child);
}

bool Node::IsDescendantOf(Node* node) const {
  if (node == this)
    return true;

  if (parent == nullptr)
    return false;

  return parent->IsDescendantOf(node);
}