#include "MyECS/core/World.hxx"
#include "MyECS/cmpt/Node.hxx"

#include <iostream>

using namespace My;
using namespace std;

int main() {
  World w;
  auto [e0, n0] = w.CreateEntity<Cmpt::Node>();
  auto [e1, n1] = w.CreateEntity<Cmpt::Node>();
  auto [e2, n2] = w.CreateEntity<Cmpt::Node>();
  auto [e3, n3] = w.CreateEntity<Cmpt::Node>();

  n0->entity = e0;
  n1->entity = e1;
  n2->entity = e2;
  n3->entity = e3;

  n0->AddChild(n1);
  n0->AddChild(n2);
  n2->AddChild(n3);

  return 0;
}