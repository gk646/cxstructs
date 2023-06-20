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
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace cxstructs {

/**
 * Implementation of a dynamic array similar to the java ArrayList or C++ std::vector
 */
template <typename T>
class ArrayList {
  T* arr;
  uint_fast32_t size_;
  uint_fast32_t len;
  uint_fast32_t minlen;
  void grow() {
    len *= 2;
    auto newarr = new T[len];
    std::move(arr, arr + size_, newarr);
    delete[] arr;
    arr = newarr;
    minlen = size_ / 4 < 32 ? 0 : size_ / 4;
  }
  void shrink() {
    len /= 2;
    auto newarr = new T[len];
    std::move(arr, arr + size_, newarr);
    delete[] arr;
    arr = newarr;

    minlen = size_ / 4 < 32 ? 0 : size_ / 4;
  }

 public:
  explicit ArrayList(uint_fast32_t initial_capacity = 32)
      : len(initial_capacity),
        size_(0),
        arr(new T[initial_capacity]),
        minlen(0) {}
  ~ArrayList() { delete[] arr; }
  /**
   * Allows direct access at the specified index starting with index 0 <p>
   * Negative indices can be used to access the list from the last element onwards starting with -1
   * @param T a reference to the value at the given index
   */
  T& operator[](int_fast32_t index) {
    if (index < 0) {
      int_fast32_t access = size_ + index;
      if (access >= 0) {
        return arr[access];
      }
      throw std::out_of_range("index out of bounds");
    } else if (index < size_) {
      return arr[index];
    }
    throw std::out_of_range("index out of bounds");
  }
  /**
   * Adds a element to the list
   * @param e the element to be added
   */
  void add(T e) {
    if (size_ == len) {
      grow();
    }
    arr[size_++] = e;
  }
  /**
   * Removes the first occurrence of the given element from the list
   * @param e element to be removed
   */
  void remove(T e) {
    if (size_ < minlen) {
      shrink();
    }
    for (int i = 0; i < len; i++) {
      if (arr[i] == e) {
        for (int j = i; j < len - 1; j++) {
          arr[i] = std::move(arr[i + 1]);
        }
        size_--;
        return;
      }
    }
  }
  /**
   * Removes the element at the given index
   * @param index index of removal
   */
  void removeAt(uint_fast32_t index) {
    if (index >= size_) {
      throw std::out_of_range("index out of bounds");
    }
    if (size_ < minlen) {
      shrink();
    }
    for (uint_fast32_t i = index; i < size_ - 1; ++i) {
      arr[i] = std::move(arr[i + 1]);
    }
    --size_;
  }
  /**
 *
 * @return the current size of the list
 */
  [[nodiscard]] inline uint_fast32_t size() const { return size_; }
  /**
   * Clears the list of all its elements <br>
   * Resets the length back to its default value(16)
   */
  void clear() {
    minlen = 0;
    size_ = 0;
    len = 16;
    delete[] arr;
    arr = new T[len];
  }
  /**
   * Provides access to the underlying array
   * Use with caution!
   * @return a pointer to the data array
   */
  T* get_raw() { return arr; }
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
  Iterator begin() { return Iterator(this->arr); }
  Iterator end() { return Iterator(arr + size_); }
  friend std::ostream& operator<<(std::ostream& os, const ArrayList<T>& list) {
    if (list.size_ == 0) {
      return os << "[]";
    }
    os << "[" << list.arr[0];

    for (int i = 1; i < list.size_; i++) {
      os << "," << list.arr[i];
    }
    return os << "]";
  }
  static void TEST() {
    std::cout << "TESTING ARRAY LIST" << std::endl;

    std::cout << "   Testing remove..." << std::endl;
    ArrayList<int> list1;
    list1.add(5);
    list1.add(10);
    list1.add(15);

    list1.remove(10);
    assert(list1.size() == 2);
    assert(list1[1] == 15);

    std::cout << "   Testing List access..." << std::endl;
    assert(list1[0] == 5);
    assert(list1[-1] == 15);
    assert(list1[-2] == 5);
    assert(list1[1] = 15);

    list1.clear();

    list1.add(5);
    list1.add(10);
    list1.add(15);

    std::cout << "   Testing iterator..." << std::endl;
    int checkNum = 0;
    for (auto& num : list1) {
      checkNum += 5;
      assert(num == checkNum);
    }
    assert(checkNum == 15);

    list1.clear();
    std::cout << "   Testing resizing..." << std::endl;
    for (int i = 0; i < 10000; i++) {
      list1.add(i);
    }
    for (int i = 0; i < 10000; i++) {
      list1.remove(i);
    }
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_ARRAYLIST_H
