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
 * Used in QuadTree.h
*/
namespace cxstructs {
/**
 * <h2>vec</h2>
 * This is an implementation of a dynamic array data structure, similar to the <code>ArrayList</code> in Java or <code>std::vector</code> in C++.
 * <br><br>
 * <p>A dynamic array is a random access, variable-n_elem list data structure that allows elements to be added or removed.
 * It provides the capability to index into the list, push_back elements to the end, and erase elements from the end in a time-efficient manner.</p>
 */
template <typename T, bool UseCXPoolAllocator = true>
class vec {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = uint_32_cx;
  using iterator = T*;
  using const_iterator = const T*;

 private:
  using Allocator =
      typename std::conditional<UseCXPoolAllocator,
                                CXPoolAllocator<T, sizeof(T) * 33, 1>,
                                std::allocator<T>>::type;
  Allocator alloc;
  T* arr_;
  size_type size_;
  size_type len_;

  bool is_trivial_destr = std::is_trivially_destructible<T>::value;
  inline void grow() {
    len_ *= 2;

    T* n_arr = alloc.allocate(len_);

    std::uninitialized_move(arr_, arr_ + size_, n_arr);

    // Destroy the original objects
    if (!is_trivial_destr) {
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
    if (!is_trivial_destr) {
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
  inline explicit vec(uint_32_cx n_elem = 32)
      : len_(n_elem), size_(0), arr_(alloc.allocate(n_elem)) {}
  inline vec(uint_32_cx n_elem, const T val)
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
  inline vec(uint_32_cx n_elem, fill_form form)
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
   * Constructs a new vector by copying from the given pointer
   * Does not take ownership of the pointer
   * @param data
   * @param n_elem
   */
  inline explicit vec(T* data, uint_32_cx n_elem)
      : size_(n_elem), len_(n_elem * 2) {
    arr_ = new T[len_];
    std::copy(data, data + n_elem, arr_);
  }
  /**
   * Initializer list constructor<p>
   * vec vec1 = {1,2,3};<p>
   * vec vec2{1,2,3};
   * @param init_list init list elements
   */
  inline vec(std::initializer_list<T> init_list)
      : size_(init_list.size()),
        len_(init_list.size() * 10),
        arr_(alloc.allocate(init_list.size() * 10)) {
    std::copy(init_list.begin(), init_list.end(), arr_);
  }
  inline vec(const vec<T>& o) : size_(o.size_), len_(o.len_) {
    arr_ = alloc.allocate(len_);
    if (is_trivial_destr) {
      std::copy(o.arr_, o.arr_ + o.size_, arr_);
    } else {
      std::uninitialized_copy(o.arr_, o.arr_ + o.size_, arr_);
    }
  }
  inline vec& operator=(const vec<T>& o) {
    if (this != &o) {
      //ugly allocator syntax but saves a lot when using e.g. vec<float>
      if (!is_trivial_destr) {
        for (uint_32_cx i = 0; i < len_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, len_);

      size_ = o.size_;
      len_ = o.len_;
      arr_ = alloc.allocate(len_);

      if (is_trivial_destr) {
        std::copy(o.arr_, o.arr_ + o.size_, arr_);
      } else {
        std::uninitialized_copy(o.arr_, o.arr_ + o.size_, arr_);
      }
    }
    return *this;
  }
  //move constructor
  inline vec(vec&& o) noexcept : arr_(o.arr_), size_(o.size_), len_(o.len_) {
    //leve other in previous state
    o.arr_ = nullptr;  // PREVENT DOUBLE DELETION!
  }
  //move assignment
  vec& operator=(vec&& o) noexcept {
    if (this != &o) {
      if (!is_trivial_destr) {
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
  inline ~vec() {
    if (!is_trivial_destr) {
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
  [[nodiscard]] inline T& operator[](const uint_32_cx& index) const noexcept {
    return arr_[index];
  }
  /**
   * Allows direct access at the specified index starting with index 0 <p>
   * Negative indices can be used to access the list from the last element onwards starting with -1
   * Throws std::out_of_range on invalid index
   * @param T a reference to the value at the given index
   */
  [[nodiscard]] inline T& at(const int_32_cx& index) const noexcept {
    if (index < 0) {
      CX_ASSERT(size_ + index >= 0 && "index out of bounds");
      return arr_[size_ + index];
    } else {
      CX_ASSERT(index < size_ && "index out of bounds");
      return arr_[index];
    }
  }
  /**
   * Adds a element to the list
   * @param e the element to be added
   */
  inline void push_back(const T& e) noexcept {
    if (size_ == len_) {
      grow();
    }
    arr_[size_++] = e;
  }
  [[nodiscard]] inline T& front() const noexcept { return arr_[0]; }
  [[nodiscard]] inline T& back() const noexcept { return arr_[size_ - 1]; }
  /**
   * Construct a new T element at the end of the list
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace_back(Args&&... args) noexcept {
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
    CX_WARNING("trying to shrink already fitting vec");
  }
  /**
  * Removes the last element of the vec.
  * Reduces the size by one.
  */
  [[nodiscard]] inline T pop_back() {
    CX_ASSERT(size_ > 0 && "out of bounds");
    return arr_[--size_];
  }
  /**
   * Returns and removes the first element of the vec<p>
   * This is rather slow method(O(n)) use a Queue if you call it frequently
   * @return first element as value
   */
  [[nodiscard]] inline T pop_front() {
    CX_ASSERT(size_ > 0 && "out of bounds");
    T val = arr_[0];
    std::move(arr_ + 1, arr_ + size_--, arr_);
    return val;
  }
  /**
   * Returns and removes the element at index i of the vec<p>
   * This is rather slow method(O(n-i))
   * @return first element as value
   */
  [[nodiscard]] inline T pop(const uint_32_cx& i) {
    T val = arr_[i];
    std::move(arr_ + i + 1, arr_ + size_--, arr_ + i);
    return val;
  }
  /**
   * Removes the first occurrence of the given element from the list
   * @param e element to be removed
   */
  inline void erase(const T& e) {
#pragma omp simd linear(i : 1)
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
    CX_ASSERT(index < len_ && "index out of bounds");
    std::move(arr_ + index + 1, arr_ + size_--, arr_ + index);
  }
  /**
 *
 * @return the current n_elem of the list
 */
  [[nodiscard]] inline uint_32_cx size() const noexcept { return size_; }
  [[nodiscard]] inline uint_32_cx capacity() const noexcept { return len_; }
  inline void reserve(uint_32_cx new_capacity) noexcept {
    if (len_ < new_capacity) {
      len_ = new_capacity;

      T* n_arr = alloc.allocate(len_);

      std::uninitialized_move(arr_, arr_ + size_, n_arr);

      // Destroy the original objects
      if (!is_trivial_destr) {
        for (size_t i = 0; i < size_; i++) {
          std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
        }
      }
      alloc.deallocate(arr_, size_);

      arr_ = n_arr;
    }
  }
  /**
   * Clears the list of all its elements <br>
   * Resets the length back to its starting value
   */
  inline void clear() noexcept {
    if (!is_trivial_destr) {
      for (uint_32_cx i = 0; i < len_; i++) {
        std::allocator_traits<Allocator>::destroy(alloc, &arr_[i]);
      }
    }
    alloc.deallocate(arr_, len_);
    size_ = 0;
    len_ = 32;
    arr_ = alloc.allocate(32);
  }
  /**
   * Provides access to the underlying array which can be used for sorting
   * Use with caution! Valid data is only guaranteed from 0 up to n_elem
   * @return a pointer to the data array
   */
  [[nodiscard]] inline T* get_raw() const noexcept { return arr_; }
  /**
   * Searches the whole vector starting from the beginning (default)
   * @param val value to search for
   * @param startFront whether to start from the front
   * @return true if the value was found
   */
  [[nodiscard]] inline bool contains(const T& val,
                                     bool startFront = true) const noexcept {
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
  inline void append(const vec<T>& vec) {
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
  inline void append(const vec<T>& list, uint_32_cx endIndex,
                     uint_32_cx startIndex = 0) {
    CX_ASSERT(startIndex < endIndex || endIndex <= list.size_,
              "index out of bounds");
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
  inline void print(const std::string& prefix = "") {
    if (!prefix.empty()) {
      std::cout << prefix << std::endl;
      std::cout << "   ";
    }
    std::cout << *this << std::endl;
  }
 friend std::ostream& operator<<(std::ostream& os, const vec& o) {
    if (o.size_ == 0) {
      return os << "[]";
    }
    os << "[" << o.arr_[0];

    for (int i = 1; i < o.size_; i++) {
      os << "," << o.arr_[i];
    }
    return os << "]";
  }
  /**
   * Performs an empty check
   * @return true if the vec is empty
   */
  [[nodiscard]] inline bool empty() const { return size_ == 0; }
  class Iterator {
    T* ptr;

   public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;

    inline explicit Iterator(T* p) noexcept : ptr(p) {}

    inline reference operator*() const noexcept { return *ptr; }
    inline pointer operator->() const noexcept { return ptr; }

    inline Iterator& operator++() noexcept {
      ++ptr;
      return *this;
    }
    inline Iterator operator++(int) const noexcept {
      Iterator temp = *this;
      ++ptr;
      return temp;
    }
    inline Iterator& operator--() noexcept {
      --ptr;
      return *this;
    }
    inline Iterator operator--(int) noexcept {
      Iterator temp = *this;
      --ptr;
      return temp;
    }
    inline Iterator operator+(difference_type n) const noexcept {
      return Iterator(ptr + n);
    }
    inline Iterator operator-(difference_type n) const noexcept {
      return Iterator(ptr - n);
    }
    inline difference_type operator-(const Iterator& other) const noexcept {
      return ptr - other.ptr;
    }
    inline Iterator& operator+=(difference_type n) noexcept {
      ptr += n;
      return *this;
    }
    inline Iterator& operator-=(difference_type n) noexcept {
      ptr -= n;
      return *this;
    }

    inline reference operator[](difference_type n) const noexcept {
      return ptr[n];
    }

    inline bool operator==(const Iterator& other) const noexcept {
      return ptr == other.ptr;
    }
    inline bool operator!=(const Iterator& other) const noexcept {
      return ptr != other.ptr;
    }
    inline bool operator<(const Iterator& other) const noexcept {
      return ptr < other.ptr;
    }
    inline bool operator>(const Iterator& other) const noexcept {
      return ptr > other.ptr;
    }
    inline bool operator<=(const Iterator& other) const noexcept {
      return ptr <= other.ptr;
    }
    inline bool operator>=(const Iterator& other) const noexcept {
      return ptr >= other.ptr;
    }
  };
  inline Iterator begin() { return Iterator(arr_); }
  inline Iterator end() { return Iterator(arr_ + size_); }

};
}  // namespace cxstructs
#endif  // CXSTRUCTS_ARRAYLIST_H
