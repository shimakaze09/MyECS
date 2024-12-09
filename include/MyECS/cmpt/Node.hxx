#ifndef NODE_HXX
#define NODE_HXX

#include "../core/Entity.hxx"

namespace My::Cmpt {
class Node {
 public:
  Node(My::Entity* entity) : entity(entity) {}

  ~Node();

  inline My::Entity* Entity() noexcept { return entity; }

  inline const My::Entity* Entity() const noexcept { return entity; }

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

#endif  //NODE_HXX
