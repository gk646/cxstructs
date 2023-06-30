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
#include <cassert>
#include <cstdint>
#include <exception>
#include <iostream>
/**
 * A queue is a datastructure that defaults to appending elements to the end but retrieving them from the beginning.<p>
 * 1,2,3,4 -> push_back(5) -> 1,2,3,4,5 <p>
 * 1,2,3,4,5 -> pop() -> 2,3,4,5 <p>
 * Useful for when you need to process data in exactly this order.
 * If you would pop elements from the back it would become a stack.
 *
 * The difference to a DeQueue (double ended queue) is the limitation of only being able to push_back to the end and pop from the start.
 * @tparam T the datatype
 */
template <typename T>
class Queue {
  T* arr_;
  uint_32_cx len_;
  uint_32_cx back_;
  uint_32_cx front_;

  uint_32_cx minlen_;

  void resize() {
    len_ *= 2;
    auto narr = new T[len_];
    std::copy(arr_ + front_, arr_ + back_, narr);
    delete[] arr_;
    arr_ = narr;
    back_ -= front_;
    front_ = 0;
    minlen_ = size() / 4 < 32 ? 0 : size() / 4;
  }
  void shrink() {
    len_ /= 2;
    auto narr = new T[len_];
    std::copy(arr_ + front_, arr_ + back_, narr);
    delete[] arr_;
    arr_ = narr;
    back_ -= front_;
    front_ = 0;
    minlen_ = size() / 4 < 32 ? 0 : size() / 4;
  }

 public:
  explicit Queue(uint_32_cx len = 32)
      : arr_(new T[len]), len_(len), minlen_(0) {
    back_ = 0;
    front_ = 0;
  }
  ~Queue() { delete[] arr_; }
  Queue(const Queue& o)
      : back_(o.back_), len_(o.len_), front_(o.front_), minlen_(o.minlen_) {
    arr_ = new T[len_];
    std::copy(o.arr_, o.arr_ + len_, arr_);
  }
  Queue& operator=(const Queue& o) {
    if (this != &o) {
      delete[] arr_;

      front_ = o.front_;
      len_ = o.len_;
      back_ = o.back_;
      arr_ = new T[len_];

      std::copy(o.arr_, o.arr_ + len_, arr_);
    }
    return *this;
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
  inline void add(T e) {
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
  inline void emplace_back(Args&&... args) {
    if (back_ == len_) {
      resize();
    }
    arr_[back_++] = T(std::forward<Args>(args)...);
  }
  /**
   * Removes and returns the element at the front of the queue
   * @return the removed element
   */
  inline T pop() {
    if (back_ - front_ < minlen_) {
      shrink();
    }
    return arr_[front_++];
  }
  /**
   * Lets you peek at the front most element <p>
   * The position at which elements at removed
   * @return a reference to the front element
   */
  [[nodiscard]] inline T& front() const { return arr_[front_]; }
  /**
   * Lets you peek at the last element <p>
   * The position at which elements at added
   * @return a reference to the last element
   */
  [[nodiscard]] inline T& back() const {
    if (back_ > 0) {
      return arr_[back_ - 1];
    }
    throw std::out_of_range("no such element");
  }
  /**
   *
   * @return true if the queue is empty
   */
  inline bool isEmpty() { return back_ - front_ == 0; }
  /**
   *
   * @return true if the queue is full
   */
  inline bool isFull() { return back_ == len_; }
  /**
   * Clears the queue of all elements
   */
  inline void clear() {
    back_ = 0;
    front_ = 0;
    minlen_ = 0;
    delete[] arr_;
    arr_ = new T[32];
    len_ = 32;
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
    std::cout << "QUEUE TESTS" << std::endl;
    // Test default constructor
    std::cout << "  Testing default constructor..." << std::endl;
    Queue<int> q1;
    assert(q1.size() == 0);
    assert(q1.isEmpty());
    assert(q1.isFull() == false);

    // Test push_back
    std::cout << "  Testing push_back..." << std::endl;
    q1.add(5);
    assert(q1.size() == 1);
    assert(q1.isEmpty() == false);
    assert(q1.front() == 5);
    assert(q1.back() == 5);

    // Test pop
    std::cout << "  Testing pop..." << std::endl;
    int val = q1.pop();
    assert(val == 5);
    assert(q1.size() == 0);
    assert(q1.isEmpty());

    // Test non-empty constructor
    std::cout << "  Testing non-empty constructor..." << std::endl;
    Queue<int> q2(5);
    for (int i = 0; i < 5; i++) {
      q2.add(10);
    }
    assert(q2.size() == 5);
    assert(q2.isEmpty() == false);
    assert(q2.front() == 10);
    assert(q2.back() == 10);

    // Test copy constructor
    std::cout << "  Testing copy constructor..." << std::endl;
    Queue<int> q3(q2);
    assert(q3.size() == q2.size());
    assert(q3.isEmpty() == q2.isEmpty());
    assert(q3.front() == q2.front());
    assert(q3.back() == q2.back());

    // Test assignment operator
    std::cout << "  Testing assignment operator..." << std::endl;
    Queue<int> q4;
    q4 = q2;
    assert(q4.size() == q2.size());
    assert(q4.isEmpty() == q2.isEmpty());
    assert(q4.front() == q2.front());
    assert(q4.back() == q2.back());

    // Test multiple push_back/pop
    std::cout << "  Testing multiple push_back/pop..." << std::endl;
    for (int i = 0; i < 1000; i++) {
      q1.add(i);
    }
    assert(q1.size() == 1000);
    for (int i = 0; i < 1000; i++) {
      int temp = q1.pop();

      assert(temp == i);
    }
    assert(q1.size() == 0);

    // Test clear
    std::cout << "  Testing clear..." << std::endl;
    q1.clear();
    assert(q1.size() == 0);
    assert(q1.isEmpty());

    // Test push_back after clear
    std::cout << "  Testing push_back after clear..." << std::endl;
    for (int i = 0; i < 10; i++) {
      q1.add(i);
    }
    assert(q1.size() == 10);

    // Test range-based for loop
    std::cout << "  Testing range-based for loop..." << std::endl;
    int check = 0;
    for (auto num : q1) {
      assert(num == check++);
    }
  }
};
#endif  // CXSTRUCTS_QUEUE_H
