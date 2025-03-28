//
// Created by Admin on 2/03/2025.
//

#pragma once

#include <_deps/taskflow/taskflow.hpp>

namespace My::MyECS {
using Job = tf::Taskflow;
using JobHandle = tf::Task;

using JobExecutor = tf::Executor;
}  // namespace My::MyECS
