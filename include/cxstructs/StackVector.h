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
#define CX_FINISHED
#ifndef CXSTRUCTS_SRC_CXSTRUCTS_STACKARRAY_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_STACKARRAY_H_

#include "../cxconfig.h"
#include <iterator>  // For std::forward_iterator_tag

namespace cxstructs {
template <typename T, size_t N, typename size_type = uint32_t>
class StackVector {

  T data_[N];       // Stack-allocated array
  size_type size_;  // Current number of elements in the array

 public:
  StackVector() : size_(0) {}
  StackVector(size_type elems) : size_(elems) {}
  StackVector(const StackVector&) = delete;
  StackVector& operator=(const StackVector&) = delete;
  StackVector(StackVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
      : size_(other.size_) {
    if constexpr (std::is_trivially_copyable_v<T>) {
      std::memcpy(data_, other.data_, other.size_ * sizeof(T));
    } else {
      for (size_type i = 0; i < other.size_; ++i) {
        new (data_ + i) T(std::move(other.data_[i]));
        other.data_[i].~T();
      }
    }
    other.size_ = 0;
  }
  StackVector& operator=(StackVector&& other) noexcept(std::is_nothrow_move_assignable_v<T>) {
    if (this != &other) {
      clear();
      size_ = other.size_;
      if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(data_, other.data_, other.size_ * sizeof(T));
      } else {
        for (size_type i = 0; i < other.size_; ++i) {
          new (data_ + i) T(std::move(other.data_[i]));
          other.data_[i].~T();
        }
      }
      other.size_ = 0;
    }
    return *this;
  }
  ~StackVector() { clear(); }
  // Call can fail
  void push_back(const T& value) {
    CX_ASSERT(size_ < N, "Attempt to add to a full StackArray");
    if constexpr (std::is_trivially_copyable_v<T>) {
      data_[size_++] = value;
    } else {
      new (data_ + size_) T(value);
      ++size_;
    }
  }
  void push_back(T&& value) {
    CX_ASSERT(size_ < N, "Attempt to add to a full StackArray");
    if constexpr (std::is_trivially_copyable_v<T>) {
      data_[size_++] = std::move(value);
    } else {
      new (data_ + size_) T(std::move(value));
      ++size_;
    }
  }
  // Call cannot fail | Overwrites first elements and resets size on loop
  void push_back_loop(const T& value) {
    size_type index = size_ % N;
    if constexpr (!std::is_trivially_copyable_v<T>) {
      if (size_ >= N) {
        data_[index].~T();
      }
    }
    new (data_ + index) T(value);
    size_ = (size_ + 1) % N;
  }
  template <typename... Args>
  void emplace_back(Args&&... args) {
    CX_ASSERT(size_ < N, "Attempt to add to a full StackArray");
    new (data_ + size_) T(std::forward<Args>(args)...);  // Direct construction with arguments
    ++size_;
  }
  T& operator[](size_type index) {
    CX_ASSERT(index < size_, "Index out of range");
    return data_[index];
  }
  const T& operator[](size_type index) const {
    CX_ASSERT(index < size_, "Index out of range");
    return data_[index];
  }
  inline void clear() {
    if constexpr (!std::is_trivially_destructible_v<T>) {
      for (size_type i = 0; i < size_; ++i) {
        data_[i].~T();  // Call destructor for each constructed element
      }
    }
    size_ = 0;
  }
  [[nodiscard]] inline size_type size() const { return size_; }
  [[nodiscard]] bool empty() const { return size_ == 0; }
  [[nodiscard]] inline bool full() const { return size_ == N; }
  [[nodiscard]] bool contains(const T& value) const {
    for (int i = 0; i < size_; ++i) {
      if (data_[i] == value) return true;
    }
    return false;
  }
  inline void resize(size_type size) { size_ = size; }
  T& front() {
    static T dummy;
    return size_ > 0 ? data_[0] : dummy;
  }
  T& back() {
    static T dummy;
    return size_ > 0 ? data_[size_ - 1] : dummy;
  }
  const T* data() { return data_; }

  // Iterator support
  class Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    explicit Iterator(pointer ptr) : ptr_(ptr) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }

    // Prefix increment
    Iterator& operator++() {
      ptr_++;
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr_ == b.ptr_; };
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr_ != b.ptr_; };

   private:
    pointer ptr_;
  };

  Iterator erase(Iterator pos) {
    // Direct pointer arithmetic on the underlying pointers to find the index
    auto posPtr = &(*pos);                   // Convert Iterator to pointer
    size_type index = posPtr - &(*begin());  // Use underlying pointers for subtraction

    // Move elements after pos one position to the left
    std::move(posPtr + 1, &(*end()), posPtr);

    // Destroy the last element (now a duplicate) if it's not trivially
    // destructible
    if constexpr (!std::is_trivially_destructible_v<T>) {
      data_[size_ - 1].~T();
    }

    // Adjust the size
    --size_;

    // Return iterator to the next element after the erased one
    // Convert back to Iterator at the correct position
    return Iterator(data_ + index);
  }
  Iterator begin() { return Iterator(&data_[0]); }
  Iterator end() { return Iterator(&data_[size_]); }

};
}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_CXSTRUCTS_STACKARRAY_H_