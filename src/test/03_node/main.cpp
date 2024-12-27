//
// Created by Admin on 27/12/2024.
//

#include <MyECS/cmpt/Node.h>
#include <MyECS/core/World.h>

#include <iostream>

using namespace std;
using namespace My;

int main() {
  World w;
  auto [e0, n0] = w.CreateEntity<Cmpt::Node>();
  auto [e1, n1] = w.CreateEntity<Cmpt::Node>();
  auto [e2, n2] = w.CreateEntity<Cmpt::Node>();
  // auto [e3, n3] = w.CreateEntity<Cmpt::Node>();
  auto [e3] = w.CreateEntity<>();
  auto [n3] = e3->Attach<Cmpt::Node>();

  // CreateEntity and Attach will auto-bind entity
  // n0->entity = e0;
  // n1->entity = e1;
  // n2->entity = e2;
  // n3->entity = e3;

  n0->AddChild(n1);
  n0->AddChild(n2);
  n2->AddChild(n3);

  return 0;
}