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
#include "./gktime.h"

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
  inline T& operator[](int_fast32_t index) {
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
  inline void add(T e) {
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
  /**
   * Searches the whole vector starting from the beginning (default)
   * @param val value to search for
   * @param startFront whether to start from the front
   * @return true if the value was found
   */
  bool contains(T val, bool startFront = true) {
    if (startFront) {
      for (int i = 0; i < size_; i++) {
        if (arr[i] == val) {
          return true;
        }
      }
      return false;
    } else {
      for (int i = size_ - 1; i > -1; i++) {
        if (arr[i] == val) {
          return true;
        }
      }
      return false;
    }
  }
  /**
 * Appends the contents of another ArrayList at the end of this list <p>
 * If necessary, the capacity of this ArrayList is expanded
 * @param list the list to append
 */
  void append(ArrayList<T>& list) {
    while(len-size_ < list.size_){
      grow();
    }
    std::copy(list.arr, list.arr+list.size_,arr+size_);
    size_+=list.size_;
  }
  /**
 * Appends a specified range of elements from another ArrayList to this ArrayList.
 * If necessary, the capacity of this ArrayList is expanded.
 * Throws a std::out_of_range exception if the start or end indices are not valid.
 *
 * @param list the list to append
 * @param end index of the last element (exclusive)
 * @param start the index of the first element (inclusive)
 */
  void append(ArrayList<T>& list, uint_fast32_t endIndex, uint_fast32_t startIndex = 0) {
    if(startIndex >= endIndex || endIndex > list.size_){
      throw std::out_of_range("index out of bounds");
    }
    while(len-size_ < endIndex - startIndex){
      grow();
    }
    std::copy(list.arr+ startIndex, list.arr+ endIndex,arr+size_);
    size_+= endIndex - startIndex;
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

    std::cout << "   Testing contains..." << std::endl;

    list1.clear();
    list1.add(5);
    assert(list1.contains(5) == true);
    assert(list1.contains(5, false) == true);

    std::cout << "   Testing append..." << std::endl;
    list1.clear();

    list1.add(5);
    list1.add(10);
    ArrayList<int> list2;
    for (int i = 0; i < 1000000; i++) {
      list2.add(i);
    }
    assert(list2.size() == 1000000);
    gkutils::now();
    list1.append(list2);
    gkutils::printTime<std::chrono::microseconds>("after");
    assert(list1.size() ==1000002);
    assert(list2[10] == 10);

    list1.clear();

    list1.append(list2,10,1);
    int check = 1;
    for(auto num : list1){
      assert(check++ == num);
    }
    assert(list1.size()== 9);
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_ARRAYLIST_H
