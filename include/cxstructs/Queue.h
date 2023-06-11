// Copyright (c) <2023> <gk646>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#define FINISHED
#ifndef CMAKE_TEST_QUEUE_H
#define CMAKE_TEST_QUEUE_H

#include <cassert>
#include <cstdint>
#include <exception>
#include <iostream>

template <typename T> class Queue {

  T *arr_;
  uint_fast16_t len_;
  uint_fast16_t begin_;
  uint_fast16_t rear_;

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
  Queue() : arr_(new T[10]), len_(10) {
    begin_ = 0;
    rear_ = 0;
  };
  Queue(uint_fast16_t len) : arr_(new T[len]), len_(len) {
    begin_ = 0;
    rear_ = 0;
  }
  Queue(uint_fast16_t len, double val) : arr_(new T[len]), len_(len) {
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
  uint_fast16_t size() const { return begin_ - rear_; }

  void enqueue(T e) {
    if (begin_ == len_) {
      resize();
    }
    arr_[begin_++] = e;
  }

  T dequeue() {
    if (rear_ < begin_) {
      return arr_[rear_++];
    }
    throw std::out_of_range("no such element");
  }
  T &front() {
    if (begin_ > 0) {
      return arr_[begin_ - 1];
    }
    throw std::out_of_range("no such element");
  }
  T &back() {
    if (begin_ > 0) {
      return arr_[rear_];
    }
    throw std::out_of_range("no such element");
  }
  bool isEmpty() { return begin_ - rear_ == 0; }

  bool isFull() { return begin_ == len_; }
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
    // Test default constructor
    Queue<int> q1;
    assert(q1.size() == 0);
    assert(q1.isEmpty());
    assert(q1.isFull() == false);

    // Test enqueue
    q1.enqueue(5);
    assert(q1.size() == 1);
    assert(q1.isEmpty() == false);
    assert(q1.front() == 5);
    assert(q1.back() == 5);

    // Test dequeue
    int val = q1.dequeue();
    assert(val == 5);
    assert(q1.size() == 0);
    assert(q1.isEmpty());

    // Test non-empty constructor
    Queue<int> q2(5, 10);
    assert(q2.size() == 5);
    assert(q2.isEmpty() == false);
    assert(q2.front() == 10);
    assert(q2.back() == 10);

    // Test copy constructor
    Queue<int> q3(q2);
    assert(q3.size() == q2.size());
    assert(q3.isEmpty() == q2.isEmpty());
    assert(q3.front() == q2.front());
    assert(q3.back() == q2.back());

    // Test assignment operator
    Queue<int> q4;
    q4 = q2;
    assert(q4.size() == q2.size());
    assert(q4.isEmpty() == q2.isEmpty());
    assert(q4.front() == q2.front());
    assert(q4.back() == q2.back());

    // Test multiple enqueue/dequeue
    for (int i = 0; i < 10; i++) {
      q1.enqueue(i);
    }
    assert(q1.size() == 10);
    for (int i = 0; i < 10; i++) {
      int temp = q1.dequeue();

      assert(temp == i);
    }
    assert(q1.size() == 0);
    std::cout << "QUEUE TESTS PASSED" << std::endl;
  }
};
#endif // CMAKE_TEST_QUEUE_H
