namespace My {
template <typename T>
T* const Pool<T>::Request() {
  if (m_freeAddresses.empty())
    NewBlock();
  T* freeAddress = m_freeAddresses.back();
  new (freeAddress) T;
  m_freeAddresses.pop_back();
  return freeAddress;
}

template <typename T>
void Pool<T>::Recycle(T* object) {
  object->~T();
  m_freeAddresses.push_back(object);
}

template <typename T>
void Pool<T>::Reserve(size_t n) {
  size_t blockNum = n / BLOCK_SIZE + (n % BLOCK_SIZE > 0 ? 1 : 0);
  for (size_t i = m_blocks.size(); i < blockNum; ++i)
    NewBlock();
}

template <typename T>
void Pool<T>::Clear() {
  std::unordered_set<T*> freeAddressesSet(m_freeAddresses.begin(),
                                         m_freeAddresses.end());
  for (auto block : m_blocks) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
      T* address = block->data() + i;
      if (freeAddressesSet.find(address) == freeAddressesSet.end())
        address->~T();
    }
    free(block);
  }
  m_blocks.clear();
  m_freeAddresses.clear();
}

template <typename T>
void Pool<T>::NewBlock() {
  auto block = (Block*)malloc(sizeof(Block));  // won't call constructor
  m_blocks.push_back(block);
  for (size_t i = 0; i < BLOCK_SIZE; i++)
    m_freeAddresses.push_back(block->data() + i);
}
}  // namespace My