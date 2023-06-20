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

#ifndef CXSTRUCTS_SRC_CXALLOCATOR_H_
#define CXSTRUCTS_SRC_CXALLOCATOR_H_

class CXAllocator {
 private:
  char* pool;  // The memory pool
  size_t size; // The size of the pool
  size_t used; // The amount of the pool currently used

 public:
  explicit CXAllocator(size_t size) : size(size), used(0) {
    pool = new char[size];
  }

  ~CXAllocator() {
    delete[] pool;
  }

  void* allocate(size_t amount) {
    if(used + amount <= size) {
      void* ptr = pool + used;
      used += amount;
      return ptr;
    } else {
      // Not enough memory left in the pool
      return nullptr;
    }
  }

  void deallocate(void* ptr, size_t amount) {
    // This simple allocator doesn't actually free memory
  }
};


#endif  //CXSTRUCTS_SRC_CXALLOCATOR_H_
