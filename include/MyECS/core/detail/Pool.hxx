#ifndef POOL_HXX
#define POOL_HXX

#include <array>
#include <unordered_set>
#include <vector>

namespace My {
template <typename T>
class Pool {
 public:
  ~Pool() { Clear(); }

  T* const Request();
  void Recycle(T* object);
  void Reserve(size_t n);
  void Clear();

 private:
  void NewBlock();

 private:
  static constexpr size_t BLOCK_SIZE = 1024;
  using Block = std::array<T, BLOCK_SIZE>;

  std::vector<Block*> m_blocks;
  std::vector<T*> m_freeAddresses;
};
}  // namespace My

#include "Pool.inl"

#endif  // POOL_HXX