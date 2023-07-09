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

//will be done using binary heap...

#ifndef CXSTRUCTS_SRC_CXSTRUCTS_PRIORITYQUEUE_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_PRIORITYQUEUE_H_

#include <queue>
#include "../cxalgos/Search.h"
#include "../cxalgos/Sorting.h"

template <typename T, bool UseCXPoolAllocator = true>
class PriorityQueue {
  using Allocator =
      typename std::conditional<UseCXPoolAllocator,
                                CXPoolAllocator<T, sizeof(T) * 33, 1>,
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
  inline void shift(uint_32_cx index) noexcept {
    for (uint_fast32_t i = index; i < len_ - 1; i++) {
      arr_[i + 1] = arr_[i];
    }
  }

 public:
  inline explicit PriorityQueue(uint_32_cx len = 32)
      : arr_(alloc.allocate(len)), len_(len) {
    back_ = 0;
    front_ = 0;
  }
  PriorityQueue(const PriorityQueue& o)
      : back_(o.back_), len_(o.len_), front_(o.front_) {
    arr_ = alloc.allocate(len_);
    if (is_trivial_destr) {
      std::copy(o.arr_ + o.front_, o.arr_ + o.back_, arr_);
    } else {
      std::uninitialized_copy(o.arr_ + o.front_, o.arr_ + o.back_, arr_);
    }
  }
  PriorityQueue& operator=(const PriorityQueue& o) {
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
    auto index = cxalgos::binary_search_index(arr_ + front_, e, len_, true);
    shift(index);
    arr_[index] = e;
    back_++;
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
    const T val;
    std::allocator_traits<Allocator>::construct(alloc, &val,
                                                std::forward<Args>(args)...);
    auto index = cxalgos::binary_search_index(arr_ + front_, val, len_, true);
    shift(index);
    arr_[index] = std::move(val);
    back_++;
  }
  /**
   * Removes the highest priority element at the front of the queue
   */
  inline void pop() {
    cxalgos::quick_sort(arr_ + front_, len_);
    front_++;
  }
  /**
   * Returns a read/write reference to the highest priority element of the queue.
   *
   * @return a reference to the highest priority element
   */
  [[nodiscard]] inline T& top() const {
    CX_ASSERT(front_ < len_, "underflow error");
    return arr_[back_ - 1];
  }
  /**
   * Returns a read/write reference to the lowest priority element of the queue.
   *
   * @return a reference to the lowest priority element
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
  friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& q) {
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
#ifndef CX_DELETE_TESTS
#include <vector>
  static void TEST() {
    std::cout << "PRIORITY QUEUE TESTS" << std::endl;
    // Test default constructor
    std::cout << "  Testing default constructor..." << std::endl;
    PriorityQueue<int> q1;
    CX_ASSERT(q1.size() == 0);
    CX_ASSERT(q1.empty());

    // Test push
    std::cout << "  Testing push..." << std::endl;
    q1.push(5);
    CX_ASSERT(q1.size() == 1);
    CX_ASSERT(q1.empty() == false);
    std::cout << q1.top() << std::endl;
    CX_ASSERT(q1.top() == 5);

    // Test pop
    std::cout << "  Testing pop..." << std::endl;
    int val = q1.top();
    q1.pop();
    CX_ASSERT(val == 5);
    CX_ASSERT(q1.size() == 0);
    CX_ASSERT(q1.empty());

    // Test non-empty constructor
    std::cout << "  Testing non-empty constructor..." << std::endl;
    PriorityQueue<int> q2(5);
    for (int i = 0; i < 5; i++) {
      q2.push(10);
    }
    CX_ASSERT(q2.size() == 5);
    CX_ASSERT(q2.empty() == false);
    CX_ASSERT(q2.top() == 10);

    // Test copy constructor
    std::cout << "  Testing copy constructor..." << std::endl;
    PriorityQueue<int> q3(q2);
    CX_ASSERT(q3.size() == q2.size());
    CX_ASSERT(q3.empty() == q2.empty());
    CX_ASSERT(q3.top() == q2.top());

    // Test assignment operator
    std::cout << "  Testing assignment operator..." << std::endl;
    PriorityQueue<int> q4;
    q4 = q2;
    CX_ASSERT(q4.size() == q2.size());
    CX_ASSERT(q4.empty() == q2.empty());
    CX_ASSERT(q4.top() == q2.top());

    // Test multiple push/pop
    std::cout << "  Testing multiple push/pop..." << std::endl;
    for (int i = 0; i < 1000; i++) {
      q1.push(i);
    }
    CX_ASSERT(q1.size() == 1000);
    for (int i = 0; i < 1000; i++) {
      int temp = q1.top();
      q1.pop();
      CX_ASSERT(temp == i);
    }
    CX_ASSERT(q1.size() == 0);

    // Test clear
    std::cout << "  Testing clear..." << std::endl;
    q1.clear();
    CX_ASSERT(q1.size() == 0);
    CX_ASSERT(q1.empty());

    // Test push after clear
    std::cout << "  Testing push after clear..." << std::endl;
    for (int i = 0; i < 10; i++) {
      q1.push(i);
    }
    q1.push(-1);
    CX_ASSERT(q1.size() == 10);
    CX_ASSERT(q1.top() == -1);
    // Test range-based for loop
    std::cout << "  Testing range-based for loop..." << std::endl;
    int check = 0;
    for (auto num : q1) {
      CX_ASSERT(num == check++);
    }
    q1.clear();
    std::priority_queue<int, std::vector<int>, std::greater<>> frontier;
    srand(1000);
    for (uint_fast32_t i = 0; i < 10; i++) {
      auto ran = rand();
      frontier.push(ran);
      q1.push(ran);
    }
    CX_ASSERT(frontier.top() == q1.top());
    q1.pop();
    frontier.pop();
    CX_ASSERT(frontier.top() == q1.top());
    q1.pop();
    frontier.pop();
    CX_ASSERT(frontier.top() == q1.top());
  }
#endif
};
#endif  //CXSTRUCTS_SRC_CXSTRUCTS_PRIORITYQUEUE_H_
