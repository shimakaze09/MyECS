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

 public:
  T* const Request() {
    if (m_freeAddresses.empty())
      NewBlock();
    T* freeAddress = m_freeAddresses.back();
    m_freeAddresses.pop_back();
    return freeAddress;
  }

  void Recycle(T* object) { m_freeAddresses.push_back(object); }

  void Reserve(size_t n) {
    size_t blockNum = n / BLOCK_SIZE + (n % BLOCK_SIZE > 0 ? 1 : 0);
    for (size_t i = m_blocks.size(); i < blockNum; ++i)
      NewBlock();
  }

  void Clear() {
    std::unordered_set<T*> freeAddressSet(m_freeAddresses.begin(),
                                          m_freeAddresses.end());
    for (auto block : m_blocks) {
      free(block);
    }
    m_blocks.clear();
    m_freeAddresses.clear();
  }

 private:
  void NewBlock() {
    auto block = (Block*)malloc(sizeof(Block));
    m_blocks.push_back(block);
    for (size_t i = 0; i < BLOCK_SIZE; ++i)
      m_freeAddresses.push_back(block->data() + i);
  }

 private:
  static constexpr size_t BLOCK_SIZE = 1024;
  using Block = std::array<T, BLOCK_SIZE>;

  std::vector<Block*> m_blocks;
  std::vector<T*> m_freeAddresses;
};
}  // namespace My

#endif  // POOL_HXX