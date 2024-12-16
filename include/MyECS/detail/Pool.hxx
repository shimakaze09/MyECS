#ifndef POOL_HXX
#define POOL_HXX

#include <array>
#include <unordered_set>
#include <vector>

namespace My {
template <typename T>
class Pool {
 public:
  ~Pool() { clear(); }

  T* const request();
  void recycle(T* object);
  void reserve(size_t n);
  void clear();

 private:
  void NewBlock();

 private:
  static const size_t BLOCK_SIZE = 1024;
  using Block = std::array<T, BLOCK_SIZE>;

  std::vector<Block*> blocks;
  std::vector<T*> freeAddresses;
};
}  // namespace My

#include "Pool.inl"

#endif  // POOL_HXX