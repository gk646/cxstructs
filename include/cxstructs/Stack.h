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

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include "../cxconfig.h"

//this stack is very fast and implemented natively (std::stack is using the std::vector)
//more can be up to 1.6 times faster and should be faster in any use case

namespace cxstructs {
/**
 * <h2>Stack</h2>
 * This data structure is a type of container adaptor, specifically designed to operate in a LIFO (Last In First Out) context, where elements are inserted and extracted only from the top of the container.
 * <br><br>
 * A Stack is a fundamental data structure that offers straightforward, linear data storage and retrieval. It provides functionality that is essential for numerous algorithms and applications, including parsing, tree traversal, and more.
 * <br><br>
 * The Stack is highly efficient and simple to use, primarily because its LIFO structure ensures that the element to be accessed is always at the same location (the top).
 */
template <typename T, uint_16_cx PreAllocBlocks = 1>
class Stack {
#ifdef CX_ALLOC
  using Allocator = CXPoolAllocator<T, sizeof(T) * 33, PreAllocBlocks>;
#else
  using Allocator = std::allocator<T>;
#endif
  Allocator alloc;
  T* arr_;
  uint_32_cx size_;
  uint_32_cx len_;

  inline void grow() {
    len_ *= 2;

    T* n_arr = alloc.allocate(len_);

    std::uninitialized_move(arr_, arr_ + size_, n_arr);

    // Destroy the original objects
    if (!std::is_trivially_destructible<T>::value) {
      for (size_t i = 0; i < size_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, size_);
    arr_ = n_arr;
    //as array is moved no need for delete []
  }

  inline void shrink() {
    auto old_len = len_;
    len_ = size_ * 1.5;

    T* n_arr = alloc.allocate(len_);

    std::uninitialized_move(arr_, arr_ + size_, n_arr);

    // Destroy the original objects
    if (!std::is_trivially_destructible<T>::value) {
      for (size_t i = 0; i < size_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, old_len);
    arr_ = n_arr;
  }

 public:
  /**
   * Default constructor<p>
   * Recommended to leave it at 16 due to optimizations with the allocator
   * @param n_elem number of starting elements
   */
  explicit Stack(uint_32_cx n_elems = 32)
      : len_(n_elems), size_(0), arr_(alloc.allocate(n_elems)) {}

  explicit Stack(uint_32_cx n_elems, T fillval)
      : len_(n_elems), size_(n_elems), arr_(alloc.allocate(n_elems)) {
    std::fill_n(arr_, len_, fillval);
  }
  /**
   * Initializer list constructor<p>
   * Stack stack1 = {1,2,3};<p>
   * Stack stack2{1,2,3};
   * @param init_list init list elements
   */
  Stack(std::initializer_list<T> init_list)
      : size_(init_list.size()),
        len_(init_list.size() * 10),
        arr_(alloc.allocate(init_list.size() * 10)) {
    std::copy(init_list.begin(), init_list.end(), arr_);
  }
  /**
* @brief Constructs a Stack with the specified number of elements, and initializes them using a provided function.
*
* @tparam fill_form A callable object or function that takes a single int as an argument and returns a float
* @param n_elem The number of elements in the vec.
* @param form The callable object or function used to initialize the elements of the Stack. It is invoked for each element with the element's index as an argument.
**/
  template <typename fill_form,
            typename = std::enable_if_t<
                std::is_invocable_r_v<double, fill_form, double>>>
  Stack(uint_32_cx n_elem, fill_form form)
      : len_(n_elem), size_(n_elem), arr_(alloc.allocate(n_elem)) {
    for (uint_32_cx i = 0; i < n_elem; i++) {
      arr_[i] = form(i);
    }
  }
  //copy constructor
  explicit Stack(const Stack<T>& o) : size_(o.size_), len_(o.len_) {
    arr_ = alloc.allocate(len_);
    std::copy(o.arr_, o.arr_ + size_, arr_);
  }
  Stack& operator=(const Stack<T>& o) {
    if (this != &o) {
      //ugly allocator syntax but saves a lot when using primitive types
      if (!std::is_trivially_destructible<T>::value) {
        for (uint_32_cx i = 0; i < len_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, len_);

      size_ = o.size_;
      len_ = o.len_;
      arr_ = alloc.allocate(len_);

      for (uint_32_cx i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(alloc, &arr_[i], o.arr_[i]);
      }
    }
    return *this;
  }
  //move constructor
  Stack(Stack&& o) noexcept : arr_(o.arr_), size_(o.size_), len_(o.len_) {
    //leve other in previous state
    o.arr_ = nullptr;  // PREVENT DOUBLE DELETION!
  }
  //move assignment
  Stack& operator=(Stack&& o) noexcept {
    if (this != &o) {
      if (!std::is_trivially_destructible<T>::value) {
        for (uint_32_cx i = 0; i < len_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, len_);

      arr_ = o.arr_;
      size_ = o.size_;
      len_ = o.len_;

      //other is left in previous state but invalidated
      o.arr_ = nullptr;  // PREVENT DOUBLE DELETION!
    }
    return *this;
  }
  ~Stack() {
    if (!std::is_trivially_destructible<T>::value) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
  }
  /**
   * Provides access to the underlying array
* Use with caution!
* @return a pointer to the data array

   */
  [[maybe_unused]] inline T* get_raw() { return arr_; }
  /**
   * @brief Returns the number of elements in the Stack.
   *
   * @return The number of elements in the Stack.
   */
  [[nodiscard]] inline uint_32_cx size() { return size_; }
  /**
     * @brief Pushes an element onto the Stack.
     *
     * @param value The value to be copied/moved onto the Stack.
     */
  inline void push(const T& val) {
    if (size_ == len_) {
      grow();
    }
    arr_[size_++] = val;
  }
  /**
   * Construct a new T element at the end of the Stack
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace(Args&&... args) {
    if (size_ == len_) {
      grow();
    }
    std::allocator_traits<Allocator>::construct(alloc, &arr_[size_++],
                                                std::forward<Args>(args)...);
  }
  /**
     * @brief Removes the top element from the Stack.
     *
     * Calling this function on an empty Stack causes undefined behavior.
     */
  inline void pop() {
    CX_ASSERT(size_ > 0 && "out of bounds");
    size_--;
  }
  /**
     * @brief Access the top element of the Stack.
     *
     * Calling this function on an empty Stack causes undefined behavior.
     *
     * @return Reference to the topmost element in the Stack.
     */
  [[nodiscard]] inline T& top() const {
    CX_ASSERT(size_ > 0 && "out of bounds");
    return arr_[size_ - 1];
  }
  /**
     * @brief Checks if the Stack is empty.
     *
     * @return true if the Stack is empty, false otherwise.
     */
  [[nodiscard]] inline bool empty() const { return size_ == 0; }
  /**
   * Reduces the underlying array size to something close to the actual data size.
   * This decreases memory usage.
   */
  void shrink_to_fit() const {
    if (len_ > size_ * 1.5) {
      shrink();
    }
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

#endif  // CXSTRUCTURES_STACK_H
