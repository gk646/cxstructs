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
#ifndef CXSTRUCTS_ARRAYLIST_H
#define CXSTRUCTS_ARRAYLIST_H

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <vector>
#include "../cxconfig.h"

/*This implementation is well optimized and should generally be a bit faster than the std::vector in a lot of use cases
 * Its using explicit allocator syntax to switch between the default and a custom one
*/
namespace cxstructs {
/**
 * <h2>vec</h2>
 * This is an implementation of a dynamic array data structure, similar to the <code>ArrayList</code> in Java or <code>std::vector</code> in C++.
 * <br><br>
 * <p>A dynamic array is a random access, variable-n_elem list data structure that allows elements to be added or removed.
 * It provides the capability to index into the list, push elements to the end, and erase elements from the end in a time-efficient manner.</p>
 */
template <typename T, uint_16_cx PreAllocBlocks = 1>
class vec {
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
      for (size_t i = 0; i < size_; i++) {
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
      for (size_t i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }

    alloc.deallocate(arr_, old_len);
    arr_ = n_arr;
  }

 public:
  /**
   * Default constructor<p>
   * Recommended to leave it at 32 due to optimizations with the allocator
   * @param n_elem number of starting elements
   */
  explicit vec(uint_32_cx n_elem = 32)
      : len_(n_elem), size_(0), arr_(alloc.allocate(n_elem)) {}
  vec(uint_32_cx n_elem, const T val)
      : len_(n_elem), size_(n_elem), arr_(alloc.allocate(n_elem)) {
    std::fill(arr_, arr_ + n_elem, val);
  }
  /**
* @brief Constructs a vec with the specified number of elements, and initializes them using a provided function.
*
* @tparam fill_form A callable object or function that takes a single int as an argument and returns a float
* @param n_elem The number of elements in the vec.
* @param form The callable object or function used to initialize the elements of the vec. It is invoked for each element with the element's index as an argument.
**/
  template <typename fill_form,
            typename = std::enable_if_t<
                std::is_invocable_r_v<double, fill_form, double>>>
  vec(uint_32_cx n_elem, fill_form form)
      : len_(n_elem), size_(n_elem), arr_(alloc.allocate(n_elem)) {
    for (uint_32_cx i = 0; i < n_elem; i++) {
      arr_[i] = form(i);
    }
  }
  /**
   * std::vector constructor<p>
   * vec vec1({1,2,3});<p>
   * @param vector
   */
  explicit vec(const std::vector<T>& vector)
      : len_(vector.size() * 1.5),
        size_(vector.size()),
        arr_(alloc.allocate(vector.size() * 1.5)) {
    std::copy(vector.begin(), vector.end(), arr_);
  }
  explicit vec(const std::vector<T>&& move_vector)
      : len_(move_vector.size() * 1.5),
        size_(move_vector.size()),
        arr_(alloc.allocate(move_vector.size() * 1.5)) {
    std::move(move_vector.begin(), move_vector.end(), arr_);
  }
  /**
   * Initializer list constructor<p>
   * vec vec1 = {1,2,3};<p>
   * vec vec2{1,2,3};
   * @param init_list init list elements
   */
  vec(std::initializer_list<T> init_list)
      : size_(init_list.size()),
        len_(init_list.size() * 10),
        arr_(alloc.allocate(init_list.size() * 10)) {
    std::copy(init_list.begin(), init_list.end(), arr_);
  }
  vec(const vec<T>& o) : size_(o.size_), len_(o.len_) {
    arr_ = alloc.allocate(len_);
    std::copy(o.arr_, o.arr_ + size_, arr_);
  }
  vec& operator=(const vec<T>& o) {
    if (this != &o) {
      //ugly allocator syntax but saves a lot when using e.g. vec<float>
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
  vec(vec&& o) noexcept : arr_(o.arr_), size_(o.size_), len_(o.len_) {
    //leve other in previous state
    o.arr_ = nullptr;  // PREVENT DOUBLE DELETION!
  }
  //move assignment
  vec& operator=(vec&& o) noexcept {
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
  ~vec() {
    if (!std::is_trivially_destructible<T>::value) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
  }
  /**
   * Direct access to the underlying array
   * @param index the accessed index
   * @return a reference to the value
   */
  [[nodiscard]] inline T& operator[](const uint_32_cx& index) const {
    return arr_[index];
  }
  /**
   * Allows direct access at the specified index starting with index 0 <p>
   * Negative indices can be used to access the list from the last element onwards starting with -1
   * Throws std::out_of_range on invalid index
   * @param T a reference to the value at the given index
   */
  inline T& at(const int_32_cx& index) {
    if (index < 0) {
      assert(size_ + index >= 0 && "index out of bounds");
      return arr_[size_ + index];
    } else {
      assert(index < size_ && "index out of bounds");
      return arr_[index];
    }
  }
  /**
   * Adds a element to the list
   * @param e the element to be added
   */
  inline void push(const T& e) {
    if (size_ == len_) {
      grow();
    }
    arr_[size_++] = e;
  }
  /**
   * Construct a new T element at the end of the list
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace_back(Args&&... args) {
    if (size_ == len_) {
      grow();
    }
    std::allocator_traits<Allocator>::construct(alloc, &arr_[size_++],
                                                std::forward<Args>(args)...);
  }

  /**
   * Reduces the underlying array size to something close to the actual data size.
   * This decreases memory usage.
   */
  inline void shrink_to_fit() {
    if (len_ > size_ * 1.5) {
      shrink();
    }
  }
  /**
  * Removes the last element of the stack.
  * Reduces the size by one.
  */
  [[nodiscard]] inline T& pop_back() {
    assert(size_ > 0 && "out of bounds");
    return arr_[--size_];
  }
  /**
   * Removes the first occurrence of the given element from the list
   * @param e element to be removed
   */
  inline void remove(const T& e) {
    for (uint_32_cx i = 0; i < len_; i++) {
      if (arr_[i] == e) {
        std::move(arr_ + i + 1, arr_ + size_, arr_ + i);
        size_--;
        return;
      }
    }
  }
  /**
   * Removes the element at the given index
   * @param index index of removal
   */
  inline void removeAt(const uint_32_cx& index) {
    assert(index < len_ && "index out of bounds");
    std::move(arr_ + index + 1, arr_ + size_--, arr_ + index);
  }
  /**
 *
 * @return the current n_elem of the list
 */
  [[nodiscard]] inline uint_32_cx size() const { return size_; }
  [[nodiscard]] inline uint_32_cx capacity() const { return len_; }
  void reserve(uint_32_cx new_capacity) {
    if (len_ < new_capacity) {

    }
  }
  /**
   * Clears the list of all its elements <br>
   * Resets the length back to its starting value
   */
  void clear() {
    if (!std::is_trivially_destructible<T>::value) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
    size_ = 0;
    len_ = 32;
    arr_ = alloc.allocate(len_);
  }
  /**
   * Provides access to the underlying array which can be used for sorting
   * Use with caution! Valid data is only guaranteed from 0 up to n_elem
   * @return a pointer to the data array
   */
  T* get_raw() { return arr_; }
  /**
   * Searches the whole vector starting from the beginning (default)
   * @param val value to search for
   * @param startFront whether to start from the front
   * @return true if the value was found
   */
  bool contains(const T& val, bool startFront = true) {
    if (startFront) {
      for (int i = 0; i < size_; i++) {
        if (arr_[i] == val) {
          return true;
        }
      }
      return false;
    } else {
      for (int i = size_ - 1; i > -1; i++) {
        if (arr_[i] == val) {
          return true;
        }
      }
      return false;
    }
  }
  /**
 * Appends the contents of another vec at the end of this vec <p>
 * If necessary, the capacity of this vec is expanded
 * @param vec  the vec to append
 */
  void append(vec<T>& vec) {
    while (len_ - size_ < vec.size_) {
      grow();
    }
    std::copy(vec.arr_, vec.arr_ + vec.size_, arr_ + size_);
    size_ += vec.size_;
  }
  /**
 * Appends a specified range of elements from another vec to this vec.
 * If necessary, the capacity of this vec is expanded.
 * Throws a std::out_of_range exception if the start or end indices are not valid.
 *
 * @param list the list to append
 * @param end index of the last element (exclusive)
 * @param start the index of the first element (inclusive)
 */
  void append(vec<T>& list, uint_32_cx endIndex, uint_32_cx startIndex = 0) {
    if (startIndex >= endIndex || endIndex > list.size_) {
      throw std::out_of_range("index out of bounds");
    }
    while (len_ - size_ < endIndex - startIndex) {
      grow();
    }
    std::copy(list.arr_ + startIndex, list.arr_ + endIndex, arr_ + size_);
    size_ += endIndex - startIndex;
  }
  /**
  * Attempts to print the complete list to std::cout
  * @param prefix optional prefix
  */
  void print(const std::string& prefix) {
    if (!prefix.empty()) {
      std::cout << prefix << std::endl;
      std::cout << "   ";
    }
    std::cout << this << std::endl;
  }
  /**
   * Performs an empty check
   * @return true if the vec is empty
   */
  [[nodiscard]] inline bool empty() const { return size_ == 0; }
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
  friend std::ostream& operator<<(std::ostream& os, const vec<T>& list) {
    if (list.size_ == 0) {
      return os << "[]";
    }
    os << "[" << list.arr_[0];

    for (int i = 1; i < list.size_; i++) {
      os << "," << list.arr_[i];
    }
    return os << "]";
  }

#ifndef CX_DELETE_TESTS
  static void TEST() {
    std::cout << "TESTING VEC\n";

    // Test: Testing push and erase
    std::cout << "   Testing push and erase...\n";
    vec<int> list1;
    list1.push(5);
    list1.push(10);
    list1.push(15);

    list1.remove(10);
    assert(list1.size() == 2);
    assert(list1[1] == 15);

    // Test: Testing List access
    std::cout << "   Testing List access...\n";
    assert(list1[0] == 5);
    assert(list1.at(-1) == 15);
    assert(list1.at(-2) == 5);
    assert(list1[1] == 15);

    // Test: Testing iterator
    std::cout << "   Testing iterator...\n";
    list1.clear();
    list1.push(5);
    list1.push(10);
    list1.push(15);

    int checkNum = 0;
    for (auto& num : list1) {
      checkNum += 5;
      assert(num == checkNum);
    }
    assert(checkNum == 15);

    // Test: Testing resizing and shrink_to_fit
    std::cout << "   Testing resizing and shrink_to_fit...\n";
    list1.clear();
    for (int i = 0; i < 10000; i++) {
      list1.push(i);
    }
    list1.shrink_to_fit();
    assert(list1.capacity() == list1.size() * 1.5);

    for (int i = 0; i < 10000; i++) {
      list1.remove(i);
    }
    assert(list1.size() == 0);

    // Test: Testing contains
    std::cout << "   Testing contains...\n";
    list1.clear();
    list1.push(5);
    assert(list1.contains(5) == true);
    assert(list1.contains(5, false) == true);

    // Test: Testing append
    std::cout << "   Testing append...\n";
    list1.clear();

    list1.push(5);
    list1.push(10);
    vec<int> list2;
    for (int i = 0; i < 1000000; i++) {
      list2.push(i);
    }
    assert(list2.size() == 1000000);

    list1.append(list2);
    assert(list1.size() == 1000002);
    assert(list2[10] == 10);

    list1.clear();

    list1.append(list2, 10, 1);
    int check = 1;
    for (auto num : list1) {
      assert(check++ == num);
    }
    assert(list1.size() == 9);

    // Test: Testing copy constructor
    std::cout << "   Testing copy constructor...\n";
    vec<int> list5(10);
    for (uint_32_cx i = 0; i < 10; i++) {
      list5.push(i);
    }
    vec<int> list6 = list5;  // copy constructor
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list6[i] == i);
    }

    // Test: Testing copy assignment
    std::cout << "   Testing copy assignment...\n";
    vec<int> list7(10);
    list7 = list5;  // copy assignment
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list7[i] == i);
    }

    // Test: Testing move constructor
    std::cout << "   Testing move constructor...\n";
    vec<int> list8 = std::move(list5);  // move constructor
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list8[i] == i);
    }

    // Test: Testing move assignment
    std::cout << "   Testing move assignment...\n";
    vec<int> list9(10);
    list9 = std::move(list6);  // move assignment
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list9[i] == i);
    }

    // Test: Testing pop_back()
    std::cout << "   Testing pop_back()...\n";
    list9.push(100);
    assert(list9.pop_back() == 100);
    assert(list9.size() == 10);

    // Test: Checking for memory leaks
    std::cout << "   Checking for memory leaks...\n";
    list1.clear();
    for (int i = 0; i < 10000000; i++) {
      list1.push(i);
    }
  }
#endif
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_ARRAYLIST_H
