//
// Created by Admin on 27/02/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace std;
using namespace My;

struct Position {
  float x;

  static void OnRegist() { cout << "regist Position" << endl; }

  void OnUpdate() const { cout << "position: " << x << endl; }
};

struct Velocity {
  float x;

  static void OnRegist() { cout << "regist Velocity" << endl; }

  void OnUpdate(Position* p) const {
    cout << "velocity: " << x << endl;
    p->x += x;
  }
};

struct Acceleration {
  float x;

  void OnUpdate(Velocity* v) const {
    cout << "Acceleration: " << x << endl;
    v->x += x;
  }
};

int main() {
  World w;
  w.CreateEntity<Velocity, Position, Acceleration>();
  w.Update(true);
  return 0;
}
