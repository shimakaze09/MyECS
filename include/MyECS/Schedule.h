//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "SystemFunc.h"
#include "detail/SysFuncGraph.h"
#include "detail/Job.h"

#include <MyContainer/Pool.h>

namespace My {
class Schedule {
public:
  Schedule& Order(size_t x, size_t y);
  Schedule& Order(std::string_view x, std::string_view y);
  Schedule& Order(SystemFunc* x, SystemFunc* y);

  template<typename... Args>
  SystemFunc* Request(Args&&... args) {
    SystemFunc* sysFunc = sysFuncPool.Request(std::forward<Args>(args)...);
    sysFuncs.emplace(sysFunc->HashCode(), sysFunc);
    return sysFunc;
  }

private:
  void Clear();
  SysFuncGraph GenSysFuncGraph() const;

  // SystemFunc's hashcode to pointer of SystemFunc
  std::unordered_map<size_t, SystemFunc*> sysFuncs;

  // SystemFunc's hashcode to SystemFunc's hashcode
  // parent to children
  std::unordered_map<size_t, size_t> sysFuncOrder;

  Pool<SystemFunc> sysFuncPool;

  friend class World;
};
}
