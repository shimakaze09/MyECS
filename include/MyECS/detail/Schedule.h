//
// Created by Admin on 2/03/2025.
//

#pragma once

#include <MyBL/Pool.h>

#include <MyDP/Basic/xSTL/xMap.h>

#include "Job.h"

namespace My {
struct Schedule {
  void Clear();
  bool IsDAG() const noexcept;
  bool GenJobGraph(Job& job) const;
  Job* RequestJob(const std::string& name);

  struct RW_Jobs {
    std::vector<Job*> pre_readers;
    std::set<Job*> writers;
    std::vector<Job*> post_readers;
  };

  std::unordered_map<size_t, RW_Jobs> id2rw;
  Pool<Job> jobPool;

  xMap<std::string, Job*> jobs;
  xMap<std::string, std::set<std::string>> sysOrderMap;  // to children
};
}  // namespace My
