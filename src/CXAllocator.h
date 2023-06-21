// Copyright (c) 2023 gk646
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#define F
#ifndef CXSTRUCTS_SRC_CXALLOCATOR_H_
#define CXSTRUCTS_SRC_CXALLOCATOR_H_
#pragma message( \
        "|CXAllocator.h| The custom allocator will preallocate memory when constructing the datastructure to speed up general use. Don't frequently create new datastructures!")

#include <iostream>
#include <memory>
#include <stack>

template <size_t BlockSize, size_t ReservedBlocks = 0>
class Pool {

  size_t size_;
  std::stack<void*> addrs_;
  std::stack<uint8_t*> blocks_;

  void allocateBlock() {
    auto* block = new uint8_t[BlockSize];

    auto total_size = BlockSize % size_ == 0 ? BlockSize : BlockSize - size_;

    for (size_t i = 0; i < total_size; i += size_) {
      addrs_.push(&block[i]);
    }

    blocks_.push(block);
  }

 public:
  explicit Pool(size_t size) : size_(size) {
    for (size_t i = 0; i < ReservedBlocks; i++) {
      allocateBlock();
    }
  }
  ~Pool() {
    while (!blocks_.empty()) {
      delete[] blocks_.top();
      blocks_.pop();
    }
  }

  void* allocate() {
    if (addrs_.empty()) {
      allocateBlock();
    }

    auto ptr = addrs_.top();
    addrs_.pop();
    return ptr;
  }
  void deallocate(void* ptr) { addrs_.push(ptr); }
};

template <typename T, size_t BlockSize = 4096, size_t ReservedBlocks = 0>
class CXPoolAllocator {
  using PoolType = Pool<BlockSize, ReservedBlocks>;
  std::shared_ptr<PoolType> pool_;

 public:
  using value_type = T;
  using is_always_equal = std::false_type;

  CXPoolAllocator() : pool_(std::make_shared<PoolType>(sizeof(T))) {}
  CXPoolAllocator(const CXPoolAllocator& other) = default;
  CXPoolAllocator(CXPoolAllocator&& other) noexcept = default;
  CXPoolAllocator& operator=(const CXPoolAllocator& other) = default;
  CXPoolAllocator& operator=(CXPoolAllocator&& other) noexcept = default;
  ~CXPoolAllocator() { pool_.release(); }
  T* allocate(size_t n) {
    if (n > 1) {
      return static_cast<T*>(malloc(sizeof(T) * n));
    }

    return static_cast<T*>(pool_->allocate());
  }
  void deallocate(T* ptr, size_t n) {
    if (n > 1) {
      free(ptr);
      return;
    }

    pool_->deallocate(ptr);
  }
};

#endif  //CXSTRUCTS_SRC_CXALLOCATOR_H_
