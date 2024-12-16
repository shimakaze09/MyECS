namespace My {
template <typename T>
T* const Pool<T>::request() {
  if (freeAddresses.empty())
    NewBlock();
  T* freeAdress = freeAddresses.back();
  new (freeAdress) T;
  freeAddresses.pop_back();
  return freeAdress;
}

template <typename T>
void Pool<T>::recycle(T* object) {
  object->~T();
  freeAddresses.push_back(object);
}

template <typename T>
void Pool<T>::reserve(size_t n) {
  size_t blockNum = n / BLOCK_SIZE + (n % BLOCK_SIZE > 0 ? 1 : 0);
  for (size_t i = blocks.size(); i < blockNum; i++)
    NewBlock();
}

template <typename T>
void Pool<T>::clear() {
  std::unordered_set<T*> freeAdressesSet(freeAddresses.begin(),
                                         freeAddresses.end());
  for (auto block : blocks) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
      T* adress = block->data() + i;
      if (freeAdressesSet.find(adress) == freeAdressesSet.end())
        adress->~T();
    }
    free(block);
  }
  blocks.clear();
  freeAddresses.clear();
}

template <typename T>
void Pool<T>::NewBlock() {
  auto block = (Block*)malloc(sizeof(Block));  // won't call constructor
  blocks.push_back(block);
  for (size_t i = 0; i < BLOCK_SIZE; i++)
    freeAddresses.push_back(block->data() + i);
}
}  // namespace My