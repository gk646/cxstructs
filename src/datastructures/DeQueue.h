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
#ifndef CXSTRUCTS_SRC_DATASTRUCTURES_DEQUEUE_H_
#define CXSTRUCTS_SRC_DATASTRUCTURES_DEQUEUE_H_

#include <algorithm>
#include <cassert>
#include <cstdint>

namespace cxstructs {
/**
 * <h2>DeQueue</h2> is a double ended queue. It functions very similar to the normal queue but is different in that it allows for retrieval and addition to both ends.
 * Like the Queue this implementation also uses an array to manage the data.
 */
template <typename T>
class DeQueue {
  T* arr_;
  uint_fast32_t mid_;
  int_fast32_t front_;
  int_fast32_t back_;

  uint_fast32_t len_;
  uint_fast32_t minlen_;

  void grow() {
    len_ *= 2;
    auto o_mid = mid_;
    mid_ = len_ / 2;
    auto n_arr = new T[len_];
    std::move(arr_ + o_mid - front_, arr_ + o_mid + back_,
              n_arr + mid_ - front_);
    delete[] arr_;
    arr_ = n_arr;
    minlen_ = size() / 4 < 16 ? 0 : size() / 4;
  }

  void shrink() {
    if ((front_ + back_) / 2 == 1) {
      len_ /= 2;
      auto o_mid = mid_;
      mid_ = len_ / 2;
      auto n_arr = new T[len_];
      std::move(arr_ + o_mid - front_, arr_ + o_mid + back_,
                n_arr + mid_ - (front_ + back_) / 2);
      delete[] arr_;
      arr_ = n_arr;
      front_ = back_ = (front_ + back_) / 2;

      back_++;
      minlen_ = size() / 4 < 16 ? 0 : size() / 4;
      std::cout<< len_ << std::endl;
    }

  }

 public:
  explicit DeQueue(uint_fast32_t initialSize = 16)
      : mid_(initialSize / 2),
        front_(1),
        back_(0),
        len_(initialSize),
        minlen_(0) {
    arr_ = new T[initialSize];
  }
  ~DeQueue() { delete[] arr_; }
  DeQueue(const DeQueue& o)
      : mid_(o.mid_),
        front_(o.front_),
        back_(o.back_),
        len_(o.len_),
        minlen_(o.minlen_) {
    arr_ = new T[len_];
    std::copy(o.arr_, o.arr_ + len_, arr_);
  }
  template <typename... Args>
  inline void emplace_front(Args&&... args) {
    if (front_ == mid_) {
      grow();
    }
    arr_[mid_ - front_++] = T(std::forward<Args>(args)...);
  }
  void add_front(T e) {
    if (front_ == mid_) {
      grow();
    }
    arr_[mid_ - front_++] = e;
  }
  void add_back(T e) {
    if (mid_ == back_) {
      grow();
    }
    arr_[mid_ + back_++] = e;
  }
  T& pop_back() {
    if (front_ + back_ < minlen_) {
      shrink();
    }
    return arr_[mid_ + back_-- - 1];
  }
  T& pop_front() {
    if (front_ + back_ < minlen_) {
      shrink();
    }
    return arr_[mid_ - front_-- + 1];
  }
  T& back() { return arr_[mid_ + back_ - 1]; }
  T& front() { return arr_[mid_ - front_ + 1]; }
  /**
 *
 * @return the current size of the dequeue
 */
  [[nodiscard]] uint_fast32_t size() const { return back_ + front_-1; }
  /**
   * Provides access to the underlying array
   * Use with caution!
   * @return a pointer to the data array
   */
  T* get_raw() { return arr_; }
  friend std::ostream& operator<<(std::ostream& os, DeQueue& q) {
    if (q.size() == 0) {
      return os << "[]";
    }
    os << "[" << q.front();
    for (int i = 2; i < q.back_ + q.front_; i++) {
      os << "," << q.arr_[q.mid_ - q.front_ + i];
    }
    return os << "]";
  }

  static void TEST() {
    std::cout << "TESTING DEQUEUE" << std::endl;

    // Testing add_front
    std::cout << "   Testing add_front..." << std::endl;
    DeQueue<int> de_queue;
  
    for (int i = 0; i < 100; i++) {
      de_queue.add_front(i);
      assert(de_queue.front() == i);
    }
    
    // Testing add_back
    std::cout << "   Testing add_back..." << std::endl;
    for (int i = 100; i < 200; i++) {
      de_queue.add_back(i);
      assert(de_queue.back() == i);
    }
   
    // Testing pop_front
    std::cout << "   Testing pop_front..." << std::endl;
    for (int i = 99; i >= 0; i--) {
      assert(de_queue.front() == i);
      de_queue.pop_front();
    }
    
    // Testing pop_back
    std::cout << "   Testing pop_back..." << std::endl;
    for (int i = 199; i >= 100; i--) {
      assert(de_queue.back() == i);
      de_queue.pop_back();
    }

    // Testing edge case: adding and removing a large number of elements
    std::cout << "   Testing edge case: adding and removing a large number of "
                 "elements..."
              << std::endl;
    
    for (int i = 0; i < 100000; i++) {
      de_queue.add_back(i);
    }

    for (int i = 0; i < 100000; i++) {

      assert(de_queue.size()== 100000-i);
      assert(de_queue.pop_front() == i);
    }

    assert(de_queue.size() == 0);
  }
};
}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_DEQUEUE_H_
