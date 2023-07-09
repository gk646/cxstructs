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
#ifndef CXSTRUCTS_SRC_DATASTRUCTURES_DEQUEUE_H_
#define CXSTRUCTS_SRC_DATASTRUCTURES_DEQUEUE_H_

#include <algorithm>

#include <cstdint>
#include <iostream>
#include "../cxconfig.h"

namespace cxstructs {
/**
 * <h2>DeQueue</h2> is a double ended queue. It functions very similar to the normal queue but is different in that it allows for retrieval and addition to both ends.
 * Like the Queue this implementation also uses an array to manage the data.
 */
template <typename T>
class DeQueue {
  T* arr_;
  uint_32_cx mid_;
  int_32_cx front_;
  int_32_cx back_;

  uint_32_cx len_;
  uint_32_cx minlen_;

  void grow() {
    len_ *= 2;
    auto o_mid = mid_;
    mid_ = len_ / 2;
    auto n_arr = new T[len_];
    std::move(arr_ + o_mid - front_, arr_ + o_mid + back_,
              n_arr + mid_ - front_);
    delete[] arr_;
    arr_ = n_arr;
    minlen_ = size() / 4 < 32 ? 0 : size() / 4;
  }

  void shrink() {
    len_ /= 2;
    auto o_mid = mid_;
    mid_ = len_ / 2;
    auto n_arr = new T[len_];

    //recentering the array
    std::move(arr_ + o_mid - front_, arr_ + o_mid + back_,
              n_arr + mid_ - (front_ + back_ + 1) / 2);

    delete[] arr_;
    arr_ = n_arr;
    int_32_cx sum = front_ + back_;
    if (sum % 2 == 0) {
      front_ = back_ = sum / 2;
    } else {
      front_ = back_ = (sum + 1) / 2;
      back_--;
    }

    minlen_ = size() / 4 < 32 ? 0 : size() / 4;
  }

 public:
  explicit DeQueue(uint_32_cx initialSize = 32)
      : mid_(initialSize / 2),
        front_(1),
        back_(0),
        len_(initialSize),
        minlen_(0),
        arr_(new T[initialSize]) {}

  DeQueue(const DeQueue& o)
      : mid_(o.mid_),
        front_(o.front_),
        back_(o.back_),
        len_(o.len_),
        minlen_(o.minlen_) {
    arr_ = new T[len_];
    std::copy(o.arr_, o.arr_ + len_, arr_);
  }
  DeQueue& operator=(const DeQueue& o) noexcept {
    if (this != &o) {
      delete[] arr_;
      len_ = o.len_;
      front_ = o.front_;
      back_ = o.back_;
      minlen_ = o.minlen_;
      arr_ = new T[len_];
      std::copy(o.len_, o.len_ + len_, arr_);
    }
    return *this;
  }
  //move copy
  DeQueue(const DeQueue&& o)
      : mid_(o.mid_),
        front_(o.front_),
        back_(o.back_),
        len_(o.len_),
        minlen_(o.minlen_),
        arr_(std::move(o.arr_, arr_)) {}
  //move assign operator
  DeQueue& operator=(DeQueue&& o) noexcept {
    if (this != &o) {
      delete[] arr_;

      arr_ = std::move(o.arr_);
      len_ = o.len_;
      front_ = o.front_;
      back_ = o.back_;
      minlen_ = o.minlen_;
    }
    return *this;
  }
  ~DeQueue() { delete[] arr_; }
  /**
 * Emplaces an element at the front of the deque.
 * Constructs the element in-place using the given arguments.
 * @param args Arguments to forward to the constructor of the element.
 */
  template <typename... Args>
  inline void emplace_front(Args&&... args) {
    if (front_ == mid_) {
      grow();
    }
    arr_[mid_ - front_++] = T(std::forward<Args>(args)...);
  }
  /**
 * Emplaces an element at the back of the deque.
 * Constructs the element in-place using the given arguments.
 * @param args Arguments to forward to the constructor of the element.
 */
  template <typename... Args>
  inline void emplace_back(Args&&... args) {
    if (front_ == mid_) {
      grow();
    }
    arr_[mid_ - front_++] = T(std::forward<Args>(args)...);
  }
  /**
 * Adds an element to the front of the deque.
 * @param e The element to be added.
 */
  inline void add_front(const T& e) {
    if (front_ == mid_) {
      grow();
    }
    arr_[mid_ - front_] = e;
    front_++;
  }
  /**
 * Adds an element to the back of the deque.
 * @param e The element to be added.
 */
  inline void add_back(const T& e) {
    if (mid_ == back_) {
      grow();
    }
    arr_[mid_ + back_] = e;
    back_++;
  }
  /**
 * Removes and returns an element from the back of the deque.
 * @return The removed element.
 */
  T pop_back() {
    if (back_ + front_ < minlen_) {
      shrink();
    }
    back_--;
    return arr_[mid_ + back_];
  }
  /**
 * Removes and returns an element from the front of the deque.
 * @return The removed element.
 */
  T pop_front() {
    if (back_ + front_ < minlen_) {
      shrink();
    }
    front_--;
    return arr_[mid_ - front_];
  }
  /**
 * Accesses the last element in the deque.
 * @return The reference to the last element in the deque.
 */
  [[nodiscard]] inline T& back() const { return arr_[mid_ + back_ - 1]; }
  /**
 * Accesses the first element in the deque.
 * @return The reference to the first element in the deque.
 */
  [[nodiscard]] inline T& front() const { return arr_[mid_ - front_ + 1]; }
  /**
 *
 * @return the current n_elem of the dequeue
 */
  [[nodiscard]] inline uint_32_cx size() const { return back_ + front_ - 1; }
  /**
   * Clears the queue of all elements
   */
  void clear() {
    back_ = 0;
    front_ = 1;
    minlen_ = 0;
    mid_ = 16;
    len_ = 32;
    delete[] arr_;
    arr_ = new T[32];
  }
  friend std::ostream& operator<<(std::ostream& os, DeQueue& q) {
    if (q.size() == 0) {
      return os << "[]";
    }
    os << "[" << q.front();
    for (uint_32_cx i = 2; i < q.back_ + q.front_; i++) {
      os << "," << q.arr_[q.mid_ - q.front_ + i];
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
  Iterator begin() { return Iterator(arr_ + mid_ - front_ + 1); }
  Iterator end() { return Iterator(arr_ + mid_ + back_); }
};
}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_DEQUEUE_H_
