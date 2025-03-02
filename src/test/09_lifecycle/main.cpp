//
// Created by Admin on 27/02/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace std;
using namespace My;

struct MyCmpt {
  static void OnRegist() { cout << "OnRegist" << endl; }

  // before first update
  static void OnStartSchedule(SystemSchedule<SysType::OnStart>& schedule) {
    cout << "OnStartSchedule" << endl;
  }

  void OnStart() { cout << "OnStart" << endl; }

  static void OnUpdateSchedule(SystemSchedule<SysType::OnUpdate>& schedule) {
    cout << "OnUpdateSchedule" << endl;
  }

  void OnUpdate() { cout << "OnUpdate" << endl; }

  // after last update
  static void OnStopSchedule(SystemSchedule<SysType::OnStop>& schedule) {
    cout << "OnStopSchedule" << endl;
  }

  void OnStop() { cout << "OnStop" << endl; }
};

// struct MySystem {
//   static void OnStartSchedule(SystemSchedule& schedule) {
//     cout << "OnStartSchedule" << endl;
//   }

//   static void OnUpdateSchedule(SystemSchedule& schedule) {
//     cout << "OnUpdateSchedule" << endl;
//   }

//   static void OnStopSchedule(SystemSchedule& schedule) {
//     cout << "OnStopSchedule" << endl;
//   }
// };

int main() {
  CmptRegister::Instance().Regist<MyCmpt>();

  World w;
  w.CreateEntity<MyCmpt>();

  w.Start();
  w.Update();
  w.Update();
  w.Stop();

  return 0;
}
