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
#define CX_FINISHED
#ifndef CXSTRUCTS_QUEUE_H
#define CXSTRUCTS_QUEUE_H

#include "../cxconfig.h"
#include <memory> //For std::allocator<T>

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
template <typename T, typename Allocator = std::allocator<T>, typename size_type = uint32_t>
class Queue {
  Allocator alloc;
  T* arr_;
  size_type capacity_;
  size_type size_;
  size_type front_;

  inline void grow() noexcept {
    capacity_ *= 2;

    T* n_arr = alloc.allocate(capacity_);

    if (front_ == 0) {
      std::uninitialized_move(arr_, arr_ + size_, n_arr);
    } else {
      std::uninitialized_move(arr_ + front_, arr_ + size_, n_arr);
      std::uninitialized_move(arr_, arr_ + front_, n_arr + size_ - 1);
    }

    if (!std::is_trivial_v<T>) {
      for (size_t i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, size_);
    arr_ = n_arr;
    front_ = 0;
  }
  inline void shrink() noexcept {
    capacity_ = size_ * 1.5;

    T* n_arr = alloc.allocate(capacity_);

    if (front_ == 0) {
      std::uninitialized_move(arr_, arr_ + size_, n_arr);
    } else {
      std::uninitialized_move(arr_ + front_, arr_ + size_, n_arr);
      std::uninitialized_move(arr_, arr_ + front_, n_arr + size_);
    }

    if constexpr (!std::is_trivial_v<T>) {
      for (size_t i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, size_);
    arr_ = n_arr;
    front_ = 0;
  }

 public:
  inline explicit Queue(uint_32_cx len = 32) : arr_(alloc.allocate(len)), capacity_(len) {
    size_ = 0;
    front_ = 0;
  }
  Queue(const Queue& o) : size_(o.size_), capacity_(o.capacity_), front_(o.front_) {
    arr_ = alloc.allocate(capacity_);
    if (std::is_trivial_v<T>) {
      std::copy(o.arr_, o.arr_ + o.capacity_, arr_);
    } else {
      std::uninitialized_copy(o.arr_, o.arr_ + o.capacity_, arr_);
    }
  }
  Queue& operator=(const Queue& o) {
    if (this != &o) {
      if (!std::is_trivial_v<T>) {
        for (uint_32_cx i = 0; i < capacity_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, capacity_);

      front_ = o.front_;
      capacity_ = o.capacity_;
      size_ = o.size_;
      arr_ = alloc.allocate(capacity_);

      if (std::is_trivial_v<T>) {
        std::copy(o.arr_, o.arr_ + o.capacity_, arr_);
      } else {
        std::uninitialized_copy(o.arr_, o.arr_ + o.capacity_, arr_);
      }
    }
    return *this;
  }
  //move copy
  Queue(Queue&& o) noexcept
      : front_(o.front_), capacity_(o.capacity_), arr_(o.arr_), size_(o.size_) {
    o.arr_ = nullptr;  //avoid double deletion
    o.capacity_ = 0;
    o.size_ = 0;
  }
  //move assign operator
  Queue& operator=(Queue&& o) noexcept {
    if (this != &o) {
      if (!std::is_trivial_v<T>) {
        for (uint_32_cx i = 0; i < capacity_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, capacity_);

      arr_ = o.arr_;
      size_ = o.size_;
      capacity_ = o.capacity_;
      front_ = o.front_;

      o.arr_ = nullptr;
      o.capacity_ = 0;
      o.size_ = 0;
    }
    return *this;
  }
  inline ~Queue() {
    if constexpr (!std::is_trivial_v<T>) {
      for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, capacity_);
  }
  /**
   *
   * @return the current n_elem of the queue
   */
  [[nodiscard]] inline uint_32_cx size() const { return size_; }
  /**
   * Adds a element to the back of the queue
   * @param e the element to be added
   */
  inline void push(const T& e) noexcept {
    if (size_ == capacity_) {
      grow();
    }
    int_32_cx index = front_ + size_;
    if (index >= capacity_) {
      index = 0;
    }
    arr_[index] = e;
    size_++;
  }
  /**
   * Adds a element to the back of the queue
   * @param e the element to be added
   */
  inline void push(T&& e) noexcept {
    if (size_ == capacity_) {
      grow();
    }
    int_32_cx index = front_ + size_;
    if (index >= capacity_) {
      index = 0;
    }
    arr_[index] = std::move(e);
    size_++;
  }
  /**
   * Construct a new T element at the end of the list
   * Will produce a leaking queue if the T constructor throws an error
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace(Args&&... args) noexcept {
    if (size_ == capacity_) {
      grow();
    }
    int_32_cx index = front_ + size_;
    if (index >= capacity_) {
      index = 0;
    }
    std::allocator_traits<Allocator>::construct(alloc, &arr_[index], std::forward<Args>(args)...);
    size_++;
  }
  /**
   * Removes the element at the front of the queue
   */
  inline void pop() noexcept {
    CX_ASSERT(size_ > 0, "no such element");
    if (++front_ >= capacity_) {
      front_ = 0;
    }
    size_--;
  }
  /**
   * Returns a read-only reference to the data at the first element of the queue.
   *  The position at which elements are removed
   * @return a reference to the front element
   */
  [[nodiscard]] inline T& front() const noexcept {
    CX_ASSERT(front_ < capacity_, "underflow error");
    return arr_[front_];
  }
  /**
   * Returns a read/write reference to the data at the first element of the queue.
   *  The position at which elements are removed
   * @return a reference to the front element
   */
  [[nodiscard]] inline T& front() noexcept {
    CX_ASSERT(front_ < capacity_, "underflow error");
    return arr_[front_];
  }
  /**
   *Returns a read-only reference to the data at the first element of the queue.
   * The position at which elements are added
   * @return a reference to the last element
   */
  [[nodiscard]] inline T& back() const noexcept {
    CX_ASSERT(size_ > 0, "no such element");

    int_32_cx index = front_ + size_ - 1;
    if (index >= capacity_) {
      index = 0;
    }
    return arr_[index];
  }
  /**
   *Returns a read/write reference to the data at the first element of the queue.
   * The position at which elements are added
   * @return a reference to the last element
   */
  [[nodiscard]] inline T& back() noexcept {
    CX_ASSERT(size_ > 0, "no such element");
    int_32_cx index = front_ + size_ - 1;
    if (index >= capacity_) {
      index = 0;
    }
    return arr_[index];
  }
  /**
   *
   * @return true if the queue is empty
   */
  inline bool empty() { return size_ == 0; }
  /**
   * Clears the queue of all elements
   */
  inline void clear() {
    if constexpr (!std::is_trivial_v<T>) {
      for (uint_32_cx i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    size_ = 0;
    front_ = 0;
  }
  /**
   * Reduces the underlying array size to something close to the actual data size.
   * This decreases memory usage.
   */
  inline void shrink_to_fit() noexcept {
    if (capacity_ > size() * 1.5) {
      shrink();
    }
  }
  /**
   * The total capacity of the queue at this point
   * @return
   */
  [[nodiscard]] inline uint_32_cx capacity() const { return capacity_; }
  class Iterator {
    T* ptr;
    uint_32_cx current;
    uint_32_cx len;

   public:
    explicit Iterator(T* p, uint_32_cx start, uint_32_cx len) : len(len), ptr(p), current(start) {}
    T& operator*() { return ptr[current]; }
    Iterator& operator++() {
      if (++current >= len) {
        current = 0;
      }
      return *this;
    }
    bool operator!=(const Iterator& other) const { return current != other.current; }
    bool operator==(const Iterator& other) const { return current == other.current; }
  };
  inline Iterator begin() { return Iterator(arr_, front_, capacity_); }
  inline Iterator end() {
    int_32_cx index = front_ + size_ - 1;
    if (index >= capacity_) {
      index = 0;
    }
    return Iterator(arr_, index, capacity_);
  }
};

}  // namespace cxstructs
#endif  // CXSTRUCTS_QUEUE_H
