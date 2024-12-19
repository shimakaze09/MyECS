//
// Created by Admin on 19/12/2024.
//

#include <MyECS/World.hxx>

#include <iostream>

using namespace My;
using namespace std;

struct Position {
  float x;

  void OnUpdate() const { cout << "position: " << x << endl; }
};

struct Velocity {
  float x;

  void OnUpdate(Position* p) const {
    p->x += x;
    cout << "update Position" << endl;
  }
};

struct Acceleration {
  float x;

  void OnUpdate(Velocity* v) const {
    v->x += x;
    cout << "update Velocity" << endl;
  }
};

int main() {
  World w;
  auto [e0, v0, p0, a0] = w.CreateEntity<Velocity, Position, Acceleration>();
  w.Update(true);
  return 0;
}