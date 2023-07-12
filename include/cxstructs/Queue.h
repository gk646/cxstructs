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
#ifndef CXSTRUCTS_QUEUE_H
#define CXSTRUCTS_QUEUE_H

#include "../cxconfig.h"

#include <cstdint>
#include <exception>
#include <iostream>

namespace cxstructs {

/**
 * A queue is a datastructure that defaults to appending elements to the end but retrieving them from the beginning.<p>
 * 1,2,3,4 -> push_back(5) -> 1,2,3,4,5 <p>
 * 1,2,3,4,5 -> pop_back() -> 2,3,4,5 <p>
 * Useful for when you need to process data in exactly this order.
 * If you would pop_back elements from the back it would become a stack.
 *
 * The difference to a DeQueue (double ended queue) is the limitation of only being able to push_back to the end and pop_back from the start.
 * @tparam T the datatype
 */
template <typename T, bool UseCXPoolAllocator = true>
class Queue {
  using Allocator =
      typename std::conditional<UseCXPoolAllocator, CXPoolAllocator<T, sizeof(T) * 33, 1>,
                                std::allocator<T>>::type;
  Allocator alloc;
  T* arr_;
  uint_32_cx len_;
  uint_32_cx back_;
  uint_32_cx front_;

  bool is_trivial_destr = std::is_trivially_destructible<T>::value;

  inline void resize() {
    len_ *= 2;

    T* n_arr = alloc.allocate(len_);
    std::uninitialized_move(arr_ + front_, arr_ + back_, n_arr);

    if (!is_trivial_destr) {
      for (size_t i = 0; i < back_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, back_);
    arr_ = n_arr;
    back_ -= front_;
    front_ = 0;
  }
  inline void shrink() {
    len_ = size() * 1.5;

    T* n_arr = alloc.allocate(len_);
    std::uninitialized_move(arr_ + front_, arr_ + back_, n_arr);

    if (!is_trivial_destr) {
      for (size_t i = 0; i < back_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, back_);
    arr_ = n_arr;

    back_ -= front_;
    front_ = 0;
  }

 public:
  inline explicit Queue(uint_32_cx len = 32) : arr_(alloc.allocate(len)), len_(len) {
    back_ = 0;
    front_ = 0;
  }
  Queue(const Queue& o) : back_(o.back_), len_(o.len_), front_(o.front_) {
    arr_ = alloc.allocate(len_);
    if (is_trivial_destr) {
      std::copy(o.arr_ + o.front_, o.arr_ + o.back_, arr_);
    } else {
      std::uninitialized_copy(o.arr_ + o.front_, o.arr_ + o.back_, arr_);
    }
  }
  Queue& operator=(const Queue& o) {
    if (this != &o) {
      if (!is_trivial_destr) {
        for (uint_32_cx i = 0; i < back_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, len_);

      front_ = 0;
      len_ = o.len_;
      back_ = o.back_ - o.front_;
      arr_ = alloc.allocate(len_);

      if (is_trivial_destr) {
        std::copy(o.arr_ + o.front_, o.arr_ + o.back_, arr_);
      } else {
        std::uninitialized_copy(o.arr_ + o.front_, o.arr_ + o.back_, arr_);
      }
    }
    return *this;
  }
  inline ~Queue() {
    if (!is_trivial_destr) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
  }
  /**
   *
   * @return the current n_elem of the queue
   */
  [[nodiscard]] inline uint_32_cx size() const { return back_ - front_; }
  /**
   * Adds a element to the back of the queue
   * @param e the element to be added
   */
  inline void push(const T& e) {
    if (back_ == len_) {
      resize();
    }
    arr_[back_++] = e;
  }
  /**
   * Construct a new T element at the end of the list
   * Will produce a leaking queue if the T constructor throws an error
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace(Args&&... args) {
    if (back_ == len_) {
      resize();
    }
    std::allocator_traits<Allocator>::construct(alloc, &arr_[back_++], std::forward<Args>(args)...);
  }
  /**
   * Removes the element at the front of the queue
   */
  inline void pop() { front_++; }
  /**
   * Returns a read/write reference to the data at the first element of the queue.
   *  The position at which elements are removed
   * @return a reference to the front element
   */
  [[nodiscard]] inline T& front() const {
    CX_ASSERT(front_ < len_, "underflow error");
    return arr_[front_];
  }
  /**
   *Returns a read/write reference to the data at the first element of the %queue.
   * The position at which elements are added
   * @return a reference to the last element
   */
  [[nodiscard]] inline T& back() const {
    CX_ASSERT(back_ <= len_, "overflow error");
    return arr_[back_ - 1];
  }
  /**
   *
   * @return true if the queue is empty
   */
  inline bool empty() { return back_ - front_ == 0; }
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
    back_ = 0;
    front_ = 0;
    len_ = 32;
    arr_ = alloc.allocate(32);
  }
  /**
   * Reduces the underlying array size to something close to the actual data size.
   * This decreases memory usage.
   */
  inline void shrink_to_fit() {
    if (len_ > size() * 1.5) {
      shrink();
    }
  }
  friend std::ostream& operator<<(std::ostream& os, const Queue& q) {
    if (q.size() == 0) {
      return os << "[]";
    }
    os << "[" << q.arr_[0];
    for (uint_32_cx i = q.front_ + 1; i < q.back_; i++) {
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
  Iterator begin() { return Iterator(arr_ + front_); }
  Iterator end() { return Iterator(arr_ + back_); }
};

}  // namespace cxstructs
#endif  // CXSTRUCTS_QUEUE_H
