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
#ifndef CXSTRUCTURES_STACK_H
#define CXSTRUCTURES_STACK_H

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace cxstructs {
template <typename T>
class Stack {
  T* arr_;
  uint_fast32_t size_;
  uint_fast32_t arrlen_;
  uint_fast32_t minlen;

  void grow() {
    arrlen_ *= 2;
    auto newarr = new T[arrlen_];
    std::copy_n(arr_, size_, newarr);
    delete[] arr_;
    arr_ = newarr;
  }

  void shrink() {
    arrlen_ /= 2;
    auto newarr = new T[arrlen_];
    std::copy_n(arr_, size_, newarr);
    delete[] arr_;
    arr_ = newarr;
    minlen = size_ / 2 > 16 ? size_ / 2 : 0;
  }

 public:
  explicit Stack(uint_fast32_t initialCapacity = 16)
      : arrlen_(initialCapacity),
        size_(0),
        arr_(new T[initialCapacity]),
        minlen(0) {}

  explicit Stack(T fillVal, uint_fast32_t initialCapacity = 16)
      : arrlen_(initialCapacity),
        size_(0),
        arr_(new T[initialCapacity]),
        minlen(0) {
    std::fill_n(arr_, arrlen_, fillVal);
    size_ = arrlen_;
  }

  Stack(const Stack<T>& o)
      : arrlen_(o.arrlen_),
        size_(o.size_),
        arr_(new T[o.arrlen_]),
        minlen(o.minlen) {
    std::copy_n(o.arr_, arrlen_, arr_);
  }

  ~Stack() { delete[] arr_; }

  Stack& operator=(const Stack<T>& o) {
    if (this == &o)
      return *this;
    minlen = o.minlen;
    arrlen_ = o.arrlen_;
    size_ = o.size_;
    delete[] arr_;
    arr_ = new T[o.arrlen_];
    std::copy_n(o.arr_, arrlen_, arr_);

    return *this;
  }
  /**
   * Provides access to the underlying array
* Use with caution!
* @return a pointer to the data array

   */
  [[maybe_unused]] T* get_raw() { return arr_; }
  /**
   *
   * @return the current size of the stack
   */
  [[nodiscard]] uint_fast32_t size() { return size_; }
  /**
 * Adds an element to the top of the stack
 * @param val element to be added
 */
  void add(T val) {
    if (size_ == arrlen_) {
      grow();
    }
    arr_[size_++] = val;
  }
  /**
   * Removes a element from the top of the stack
   * @return the removed element
   */
  T& pop() {
    if (size_ == 0) {
      throw std::logic_error("cant pop from empty stack!");
    }
    if (size_ - 1 < minlen) {
      shrink();
    }
    size_--;
    return arr_[size_];
  }
  /**
   * Lets you peek at the top element
   * @return a reference to the top element
   */
  T& top() { return arr_[size_ - 1]; }

  static void TEST() {
    std::cout << "STACK TESTS" << std::endl;
    // Test default constructor
    std::cout << "  Testing default constructor..." << std::endl;
    Stack<int> stack1;
    assert(stack1.size() == 0);

    // Test constructor with fill value and initial capacity
    std::cout << "  Testing constructor with fill value and initial capacity..."
              << std::endl;
    Stack<int> stack2(1, 5);
    assert(stack2.size() == 5);
    for (int i = 0; i < 5; i++) {
      assert(stack2.top() == 1);
      stack2.pop();
    }

    // Test copy constructor
    std::cout << "  Testing copy constructor..." << std::endl;
    Stack<int> stack3(stack2);
    assert(stack3.size() == stack2.size());

    // Test assignment operator
    std::cout << "  Testing assignment operator..." << std::endl;
    Stack<int> stack4;
    stack4 = stack2;
    assert(stack4.size() == stack2.size());

    // Test add
    std::cout << "  Testing add method..." << std::endl;
    stack1.add(1);
    assert(stack1.top() == 1);
    assert(stack1.size() == 1);

    // Test pop
    std::cout << "  Testing pop method..." << std::endl;
    stack1.pop();
    assert(stack1.size() == 0);

    std::cout << "  Testing memory leak..." << std::endl;
    for (int i = 0; i < 100000; i++) {
      stack1.add(i);
    }
  }
};
}  // namespace cxstructs

#endif  // CXSTRUCTURES_STACK_H
