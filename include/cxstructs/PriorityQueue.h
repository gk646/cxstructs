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
#define FINISHED
#ifndef CXSTRUCTS_SRC_CXSTRUCTS_PRIORITYQUEUE_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_PRIORITYQUEUE_H_

#include <algorithm>
#include <iostream>
#include <queue>
#include "../cxconfig.h"

namespace cxstructs {

/**
 * @class PriorityQueue
 * @brief A priority queue implementation using a binary heap.
 *
 * This class provides a priority queue data structure, which is a container adaptor that provides constant time lookup
 * of the largest (by default) element, at the expense of logarithmic insertion and extraction. A user-defined comparator
 * can be supplied to change the ordering, e.g., using std::greater<T> would cause the smallest element to appear as the top().
 */
template <typename T, typename Compare = std::greater<T>, bool UseCXPoolAllocator = true>
class PriorityQueue {
  using Allocator =
      typename std::conditional<UseCXPoolAllocator, CXPoolAllocator<T, sizeof(T) * 33, 1>,
                                std::allocator<T>>::type;
  Allocator alloc;
  T* arr_;
  uint_32_cx len_;
  uint_32_cx size_;
  Compare comp;

  bool is_trivial_destr = std::is_trivially_destructible<T>::value;

  inline void resize() noexcept {
    len_ *= 2;

    T* n_arr = alloc.allocate(len_);
    std::uninitialized_move(arr_, arr_ + size_, n_arr);

    if (!is_trivial_destr) {
      for (size_t i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, size_);
    arr_ = n_arr;
  }
  inline void shrink() noexcept {
    auto old_len = len_;
    len_ = size_ * 1.5;

    T* n_arr = alloc.allocate(len_);
    std::uninitialized_move(arr_, arr_ + size_, n_arr);

    // Destroy the original objects
    if (!is_trivial_destr) {
      for (size_t i = 0; i < old_len; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, old_len);
    arr_ = n_arr;
  }
  inline void sift_up(uint_32_cx index) noexcept {
    auto parent = (index - 1) / 2;
    while (index != 0 && !comp(arr_[index], arr_[parent])) {
      std::swap(arr_[index], arr_[parent]);
      index = parent;
      parent = (index - 1) / 2;
    }
  }
  inline void sift_down(uint_32_cx index) noexcept {
    auto left = 2 * index + 1;
    auto right = 2 * index + 2;
    while ((left < size_ && comp(arr_[index], arr_[left])) ||
           (right < size_ && comp(arr_[index], arr_[right]))) {
      auto smallest = (right >= size_ || comp(arr_[right], arr_[left])) ? left : right;
      std::swap(arr_[index], arr_[smallest]);
      index = smallest;
      left = 2 * index + 1;
      right = 2 * index + 2;
    }
  }
  inline void heapify() noexcept {
    for (uint_fast32_t i = len_ - 1; i > -1; i--) {
      sift_down(i);
    }
  }

 public:
  /**
   * Per default is a min heap. Pass std::greater<> as comparator to get a max-heap
   * @param len initial length and expansion factor
   */
  inline explicit PriorityQueue(uint_32_cx len = 32)
      : arr_(alloc.allocate(len)), len_(len), size_(0) {}
  /**
   * @brief Constructor that initializes the priority queue with an existing array.
   *  <b>Takes ownership of the array</b>
   * @param arr Pointer to the array to copy elements from.
   * @param len The number of elements in the array.
   */
  inline explicit PriorityQueue(T*&& arr, uint_32_cx len) : arr_(arr), len_(len), size_(len) {
    heapify();
    arr = nullptr;  //avoid double deletion
  }
  /**
   * @brief Constructor that initializes the priority queue by copying the contents of an existing array.
   *
   * @param arr Pointer to the array to copy elements from.
   * @param len The number of elements in the array.
   */
  inline explicit PriorityQueue(const T* arr, uint_32_cx len)
      : arr_(alloc.allocate(len)), len_(len), size_(len) {
    if (is_trivial_destr) {
      std::copy(arr, arr + len, arr_);
    } else {
      std::uninitialized_copy(arr, arr + len, arr_);
    }
    heapify();
  }

  PriorityQueue(const PriorityQueue& o) : len_(o.len_), size_(o.size_), comp(o.comp) {
    arr_ = alloc.allocate(len_);
    if (is_trivial_destr) {
      std::copy(o.arr_, o.arr_ + o.size_, arr_);
    } else {
      std::uninitialized_copy(o.arr_, o.arr_ + o.size_, arr_);
    }
  }
  PriorityQueue& operator=(const PriorityQueue& o) {
    if (this != &o) {
      if (!is_trivial_destr) {
        for (uint_32_cx i = 0; i < len_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, len_);

      len_ = o.len_;
      size_ = o.size_;
      arr_ = alloc.allocate(len_);

      if (is_trivial_destr) {
        std::copy(o.arr_, o.arr_ + o.size_, arr_);
      } else {
        std::uninitialized_copy(o.arr_, o.arr_ + o.size_, arr_);
      }
    }
    return *this;
  }
  inline ~PriorityQueue() {
    if (!is_trivial_destr) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
  }
  /**
   *
   * @return the current n_elem of the priority-queue
   */
  [[nodiscard]] inline uint_32_cx size() const { return size_; }
  /**
   * Adds an element to the priority queue
   * @param e the element to be added
   */
  inline void push(const T& e) noexcept {
    if (size_ == len_) {
      resize();
    }
    arr_[size_] = e;
    sift_up(size_++);
  }
  /**
   * Construct a new T element at the end of the list
   * Will produce a leaking queue if the T constructor throws an error
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace(Args&&... args) noexcept {
    if (size_ == len_) {
      resize();
    }
    std::allocator_traits<Allocator>::construct(alloc, &arr_[size_], std::forward<Args>(args)...);
    sift_up(size_++);
  }
  /**
   * Removes the highest priority element from the priority queue
   */
  inline void pop() noexcept {
    CX_ASSERT(size_ > 0, "no such element");
    std::swap(arr_[0], arr_[--size_]);
    sift_down(0);
  }
  /**
   * Returns a read/write reference to the highest priority element of the queue.
   *
   * @return a reference to the highest priority element
   */
  [[nodiscard]] inline T& top() const noexcept { return arr_[0]; }
  /**
   *
   * @return true if the queue is empty
   */
  inline bool empty() { return size_ == 0; }
  /**
   * Clears the queue of all elements
   */
  inline void clear() {
    if (!is_trivial_destr) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
    size_ = 0;
    len_ = 32;
    arr_ = alloc.allocate(32);
  }
  /**
   * Reduces the underlying array size to something close to the actual data size.
   * This decreases memory usage.
   */
  inline void shrink_to_fit() noexcept {
    CX_WARNING(len_ > size_ * 1.5, "calling shrink_to_fit for no reason");
    shrink();
  }
  friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& q) {
    if (q.size() == 0) {
      return os << "[]";
    }
    os << "[" << q.arr_[0];
    for (uint_32_cx i = 1; i < q.size_; i++) {
      os << "," << q.arr_[i];
    }
    return os << "]";
  }
  class Iterator {
    T* ptr;

   public:
    explicit Iterator(T* p) : ptr(p) {}
    T& operator*() { return *ptr; }
    Iterator& operator++() {
      ++ptr;
      return *this;
    }
    bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
  };
  Iterator begin() { return Iterator(arr_); }
  Iterator end() { return Iterator(arr_ + size_); }

};
}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_CXSTRUCTS_PRIORITYQUEUE_H_
