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

#include <cassert>
#include <cstdint>
#include <exception>
#include <iostream>

template <typename T> class Queue {
  T *arr_;
  uint_fast32_t len_;
  uint_fast32_t begin_;
  uint_fast32_t rear_;

  void resize() {
    len_ = 2 * len_;
    auto narr = new T[len_];
    std::copy(arr_ + rear_, arr_ + begin_, narr);
    delete[] arr_;
    arr_ = narr;
    begin_ -= rear_;
    rear_ = 0;
  }

public:
  explicit Queue(uint_fast16_t len = 16) : arr_(new T[len]), len_(len) {
    begin_ = 0;
    rear_ = 0;
  }
  Queue(uint_fast16_t len, T val) : arr_(new T[len]), len_(len) {
    begin_ = len;
    rear_ = 0;
    std::fill(arr_, arr_ + len_, val);
  }
  ~Queue() { delete[] arr_; }
  Queue(const Queue &o) : begin_(o.begin_), len_(o.len_), rear_(o.rear_) {
    arr_ = new T[len_];
    std::copy(o.arr_, o.arr_ + len_, arr_);
  }
  Queue &operator=(const Queue &o) {
    if (this != &o) {
      delete[] arr_;

      rear_ = o.rear_;
      len_ = o.len_;
      begin_ = o.begin_;
      arr_ = new T[len_];

      std::copy(o.arr_, o.arr_ + len_, arr_);
    }
    return *this;
  }
  /**
   *
   * @return the current size of the queue
   */
  [[nodiscard]] uint_fast32_t size() const { return begin_ - rear_; }
  /**
   * Adds a element to the end of the queue
   * @param e the element to be added
   */
  void enqueue(T e) {
    if (begin_ == len_) {
      resize();
    }
    arr_[begin_++] = e;
  }
  /**
   * Removes the front element from the queue
   * @return the removed element
   */
  T dequeue() {
    if (rear_ < begin_) {
      return arr_[rear_++];
    }
    throw std::out_of_range("no such element");
  }
  /**
   * Lets you peek at the front most element
   * @return a reference to the front element
   */
  T &front() {
    if (begin_ > 0) {
      return arr_[begin_ - 1];
    }
    throw std::out_of_range("no such element");
  }
  /**
   * Lets you peek at the last element
   * @return a reference to the last element
   */
  T &back() {
    if (begin_ > 0) {
      return arr_[rear_];
    }
    throw std::out_of_range("no such element");
  }
  /**
   *
   * @return true if the queue is empty
   */
  bool isEmpty() { return begin_ - rear_ == 0; }
  /**
   *
   * @return true if the queue is full
   */
  bool isFull() { return begin_ == len_; }
  /**
   * Clears the queue of all elements
   */
  void clear() {
    begin_ = 0;
    rear_ = 0;
    delete[] arr_;
    arr_ = new T[16];
    len_ = 16;
  }
  friend std::ostream &operator<<(std::ostream &os, const Queue &q) {
    if (q.size() == 0) {
      return os << "[]";
    }
    os << "[" << q.arr_[0];
    for (uint_fast16_t i = q.rear_ + 1; i < q.begin_; i++) {
      os << "," << q.arr_[i];
    }
    return os << "]";
  }

  class iterator {
    T *ptr;

  public:
    explicit iterator(T *p) : ptr(p) {}

    T &operator*() { return *ptr; }

    iterator &operator++() {
      ++ptr;
      return *this;
    }

    bool operator!=(const iterator &other) const { return ptr != other.ptr; }
  };
  iterator begin() { return iterator(arr_ + rear_); }
  iterator end() { return iterator(arr_ + begin_); }

  static void TEST() {
    std::cout << "QUEUE TESTS" << std::endl;
    // Test default constructor
    std::cout << "  Testing default constructor..." << std::endl;
    Queue<int> q1;
    assert(q1.size() == 0);
    assert(q1.isEmpty());
    assert(q1.isFull() == false);

    // Test enqueue
    std::cout << "  Testing enqueue..." << std::endl;
    q1.enqueue(5);
    assert(q1.size() == 1);
    assert(q1.isEmpty() == false);
    assert(q1.front() == 5);
    assert(q1.back() == 5);

    // Test dequeue
    std::cout << "  Testing dequeue..." << std::endl;
    int val = q1.dequeue();
    assert(val == 5);
    assert(q1.size() == 0);
    assert(q1.isEmpty());

    // Test non-empty constructor
    std::cout << "  Testing non-empty constructor..." << std::endl;
    Queue<int> q2(5, 10);
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

    // Test multiple enqueue/dequeue
    std::cout << "  Testing multiple enqueue/dequeue..." << std::endl;
    for (int i = 0; i < 10; i++) {
      q1.enqueue(i);
    }
    assert(q1.size() == 10);
    for (int i = 0; i < 10; i++) {
      int temp = q1.dequeue();

      assert(temp == i);
    }
    assert(q1.size() == 0);

    // Test clear
    std::cout << "  Testing clear..." << std::endl;
    q1.clear();
    assert(q1.size() == 0);
    assert(q1.isEmpty());

    // Test enqueue after clear
    std::cout << "  Testing enqueue after clear..." << std::endl;
    for (int i = 0; i < 10; i++) {
      q1.enqueue(i);
    }
    assert(q1.size() == 10);

    // Test range-based for loop
    std::cout << "  Testing range-based for loop..." << std::endl;
    int check = 0;
    for(auto num : q1){
      assert(num == check++);
    }
  }
};
#endif // CXSTRUCTS_QUEUE_H
