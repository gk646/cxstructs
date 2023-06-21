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

//This implementation is well optimized (from what I can tell) and should be faster or equal than the std::vector in a lot of use cases
namespace cxstructs {
/**
 * <h2>Array List</h2>
 * This is an implementation of a dynamic array data structure, similar to <code>ArrayList</code> in Java or <code>std::vector</code> in C++.
 * <br><br>
 * <p>A dynamic array is a random access, variable-size list data structure that allows elements to be added or removed.
 * It provides the capability to index into the list, add elements to the end, and remove elements from the end in a time-efficient manner.</p>
 */
template <typename T>
class ArrayList {
  T* arr;
  uint_fast32_t size_;
  uint_fast32_t len;
  uint_fast32_t minlen;

  inline void grow() {
    len *= 1.5;
    auto newarr = new T[len];
    std::move(arr, arr + size_, newarr);
    delete[] arr;
    arr = newarr;

    minlen = size_ / 6 < 32 ? 0 : size_ / 6; // lower bound is very lenient to avoid premature downsizing
  }
  inline void shrink() {
    len /= 2;
    auto newarr = new T[len];
    std::move(arr, arr + size_, newarr);
    delete[] arr;
    arr = newarr;

    minlen = size_ / 6 < 32 ? 0 : size_ / 6;
  }

 public:
  explicit ArrayList(uint_fast32_t initial_capacity = 64)
      : len(initial_capacity),
        size_(0),
        arr(new T[initial_capacity]),
        minlen(0) {}
  ~ArrayList() { delete[] arr; }
  ArrayList(const ArrayList<T>& o)
      : size_(o.size_), len(o.len), minlen(o.minlen) {
    arr = new T[len];
    std::copy(o.arr, o.arr + size_, arr);
  }
  ArrayList& operator=(const ArrayList<T>& o) {
    if (this != &o) {
      delete[] arr;

      size_ = o.size_;
      len = o.len;
      minlen = o.minlen;
      arr = new T[size_];
      std::copy(o.arr, o.arr + size_, arr);
    }
    return *this;
  }
  //move constructor
  ArrayList(ArrayList&& o) noexcept
      : arr(o.arr), size_(o.size_), len(o.len), minlen(o.minlen) {
    o.arr = nullptr;
    o.size_ = 0;
    o.len = 0;
    o.minlen = 0;
  }
  //move assignment
  ArrayList& operator=(ArrayList&& o) noexcept {
    if (this != &o) {
      delete[] arr;

      arr = o.arr;
      size_ = o.size_;
      len = o.len;
      minlen = o.minlen;

      o.arr = nullptr;
      o.size_ = 0;
      o.len = 0;
      o.minlen = 0;
    }
    return *this;
  }
  /**
   * Direct access to the underlying array
   * @param index the accessed index
   * @return a reference to the value
   */
  [[nodiscard]] inline T& operator[](const uint_fast32_t& index) {
    return arr[index];
  }
  /**
   * Allows direct access at the specified index starting with index 0 <p>
   * Negative indices can be used to access the list from the last element onwards starting with -1
   * Throws std::out_of_range on invalid index
   * @param T a reference to the value at the given index
   */
  inline T& at(const int_fast32_t& index) {
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
    arr[size_++] = std::move(e);
  }
  /**
   * Construct a new T element at the end of the list
   * Will produce a leaking vector if the T constructor throws an error
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace_back(Args&&... args) {
    if (size_ == len) {
      grow();
    }
    arr[size_] = T(std::forward<Args>(args)...);
    ++size_;
  }
  /**
   * Removes the first occurrence of the given element from the list
   * @param e element to be removed
   */
  inline void remove(const T& e) {
    if (size_ < minlen) {
      shrink();
    }
    for (uint_fast32_t i = 0; i < len; i++) {
      if (arr[i] == e) {
        std::move(arr + i + 1, arr + size_, arr + i);
        size_--;
        return;
      }
    }
  }
  /**
   * Removes the element at the given index
   * @param index index of removal
   */
  inline void removeAt(const uint_fast32_t& index) {
    if (size_ < minlen) {
      shrink();
    }
    std::move(arr + index + 1, arr + size_, arr + index);
    --size_;
  }
  /**
 *
 * @return the current size of the list
 */
  [[nodiscard]] inline uint_fast32_t size() const { return size_; }
  /**
   * Clears the list of all its elements <br>
   * Resets the length back to its starting value
   */
  void clear() {
    minlen = 0;
    size_ = 0;
    len = 32;
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
  bool contains(const T& val, bool startFront = true) {
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
    while (len - size_ < list.size_) {
      grow();
    }
    std::copy(list.arr, list.arr + list.size_, arr + size_);
    size_ += list.size_;
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
  void append(ArrayList<T>& list, uint_fast32_t endIndex,
              uint_fast32_t startIndex = 0) {
    if (startIndex >= endIndex || endIndex > list.size_) {
      throw std::out_of_range("index out of bounds");
    }
    while (len - size_ < endIndex - startIndex) {
      grow();
    }
    std::copy(list.arr + startIndex, list.arr + endIndex, arr + size_);
    size_ += endIndex - startIndex;
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
  Iterator begin() { return Iterator(arr); }
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

    // Test 1: Testing add and remove
    std::cout << "   Test 1: Testing add and remove..." << std::endl;
    ArrayList<int> list1;
    list1.add(5);
    list1.add(10);
    list1.add(15);

    list1.remove(10);
    assert(list1.size() == 2);
    assert(list1[1] == 15);

    // Test 2: Testing List access
    std::cout << "   Test 2: Testing List access..." << std::endl;
    assert(list1[0] == 5);
    assert(list1.at(-1) == 15);
    assert(list1.at(-2) == 5);
    assert(list1[1] == 15);

    // Test 3: Testing iterator
    std::cout << "   Test 3: Testing iterator..." << std::endl;
    list1.clear();
    list1.add(5);
    list1.add(10);
    list1.add(15);

    int checkNum = 0;
    for (auto& num : list1) {
      checkNum += 5;
      assert(num == checkNum);
    }
    assert(checkNum == 15);

    // Test 4: Testing resizing
    std::cout << "   Test 4: Testing resizing..." << std::endl;
    list1.clear();
    for (int i = 0; i < 10000; i++) {
      list1.add(i);
    }
    for (int i = 0; i < 10000; i++) {
      list1.remove(i);
    }
    assert(list1.size() == 0);

    // Test 5: Testing contains
    std::cout << "   Test 5: Testing contains..." << std::endl;
    list1.clear();
    list1.add(5);
    assert(list1.contains(5) == true);
    assert(list1.contains(5, false) == true);

    // Test 6: Testing append
    std::cout << "   Test 6: Testing append..." << std::endl;
    list1.clear();

    list1.add(5);
    list1.add(10);
    ArrayList<int> list2;
    for (int i = 0; i < 1000000; i++) {
      list2.add(i);
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

    // Test 7: copy constructor
    std::cout << "   Test 7: Testing copy constructor..." << std::endl;
    ArrayList<int> list5(10);
    for (uint_fast32_t i = 0; i < 10; i++) {
      list5.add(i);
    }
    ArrayList<int> list6 = list5;  // copy constructor
    for (uint_fast32_t i = 0; i < 10; i++) {
      assert(list6[i] == i);
    }

    // Test 8: copy assignment
    std::cout << "   Test 8: Testing copy assignment..." << std::endl;
    ArrayList<int> list7(10);
    list7 = list5;  // copy assignment
    for (uint_fast32_t i = 0; i < 10; i++) {
      assert(list7[i] == i);
    }

    // Test 9: move constructor
    std::cout << "   Test 9: Testing move constructor..." << std::endl;
    ArrayList<int> list8 = std::move(list5);  // move constructor
    for (uint_fast32_t i = 0; i < 10; i++) {
      assert(list8[i] == i);
    }

    // Test 10: move assignment
    std::cout << "   Test 10: Testing move assignment..." << std::endl;
    ArrayList<int> list9(10);
    list9 = std::move(list6);  // move assignment
    for (uint_fast32_t i = 0; i < 10; i++) {
      assert(list9[i] == i);
    }

    // Test 11: Checking for memory leaks
    std::cout << "   Test 11: Checking for memory leaks..." << std::endl;
    list1.clear();
    for (int i = 0; i < 10000000; i++) {
      list1.add(i);
    }
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_ARRAYLIST_H
