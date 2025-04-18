#pragma once

#include <_deps/taskflow/taskflow.hpp>

namespace Smkz::MyECS {
using Job = tf::Taskflow;
using JobHandle = tf::Task;

using JobExecutor = tf::Executor;
}  // namespace Smkz::MyECS
