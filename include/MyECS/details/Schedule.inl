//
// Created by Admin on 8/03/2025.
//

#pragma once

namespace My::MyECS {
template <typename Func>
const SystemFunc* Schedule::RegisterEntityJob(Func&& func, std::string name,
                                              bool isParallel,
                                              ArchetypeFilter filter,
                                              CmptLocator cmptLocator,
                                              SingletonLocator singletonLocator,
                                              RandomAccessor randomAccessor) {
  return Request(std::forward<Func>(func), std::move(name), std::move(filter),
                 std::move(cmptLocator), std::move(singletonLocator),
                 std::move(randomAccessor), isParallel);
}

template <typename Func>
const SystemFunc* Schedule::RegisterChunkJob(Func&& func, std::string name,
                                             ArchetypeFilter filter,
                                             bool isParallel,
                                             SingletonLocator singletonLocator,
                                             RandomAccessor randomAccessor) {
  return Request(std::forward<Func>(func), std::move(name), std::move(filter),
                 std::move(singletonLocator), std::move(randomAccessor),
                 isParallel);
}

template <typename Func>
const SystemFunc* Schedule::RegisterJob(Func&& func, std::string name,
                                        SingletonLocator singletonLocator,
                                        RandomAccessor randomAccessor) {
  return Request(std::forward<Func>(func), std::move(name),
                 std::move(singletonLocator), std::move(randomAccessor));
}

template <typename... Args>
const SystemFunc* Schedule::Request(Args&&... args) {
  SystemFunc* sysFunc = sysFuncAllocator.allocate(1);
  new(sysFunc)SystemFunc(std::forward<Args>(args)...);
  sysFuncs.emplace(sysFunc->GetValue(), sysFunc);
  return sysFunc;
}

inline Schedule& Schedule::LockFilter(std::string_view sys) {
  sysLockFilter.insert(SystemFunc::GetValue(sys));
  return *this;
}
}  // namespace My::MyECS
