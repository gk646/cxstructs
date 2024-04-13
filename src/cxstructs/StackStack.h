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

#ifndef CXSTRUCTS_SRC_CXSTRUCTS_STACKSTACK_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_STACKSTACK_H_

#include "../cxconfig.h"
#include <iterator>  // For std::forward_iterator_tag

namespace cxstructs {

template <typename T, size_t N, typename size_type = uint32_t>
class StackStack {
  T data_[N];           // Stack-allocated array
  size_type size_ = 0;  // Current number of elements in the array

 public:
  // Call can fail
  void push(const T& value) {
    CX_ASSERT(size_ < N, "Attempt to add to a full StackStack");
    CX_STACK_ABORT_IMPL();
    if constexpr (std::is_trivially_copyable_v<T>) {
      data_[size_++] = value;
    } else {
      new (data_ + size_) T(value);
      ++size_;
    }
  }

  //Call can fail
  void push(T&& value) {
    CX_ASSERT(size_ < N, "Attempt to add to a full StackStack");
    CX_STACK_ABORT_IMPL();
    if constexpr (std::is_trivially_copyable_v<T>) {
      data_[size_++] = std::move(value);
    } else {
      new (data_ + size_) T(std::move(value));
      ++size_;
    }
  }

  //Call can fail
  template <typename... Args>
  void emplace(Args&&... args) {
    CX_ASSERT(size_ < N, "Attempt to add to a full StackStack");
    CX_STACK_ABORT_IMPL();
    new (data_ + size_) T(std::forward<Args>(args)...);
    ++size_;
  }

  auto size() -> size_type { return size_; }

  auto top() -> T& {
    CX_ASSERT(size_ > 0, "Index out of range");
    return data_[size_ - 1];
  }

  auto empty() -> bool { return size_ == 0; }

  auto pop() -> void {
    if (size_ > 0) {
      if constexpr (!std::is_trivially_destructible_v<T>) {
        data_[size_ - 1].~T();
      }
      size_--;
    }
  }

  auto clear() -> void {
    if constexpr (!std::is_trivially_destructible_v<T>) {
      for (size_type i = 0; i < size_; ++i) {
        data_[i].~T();  // Call destructor for each constructed element
      }
    }
    size_ = 0;
  }

  template <typename PtrType>
  class IteratorTemplate {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::remove_const_t<PtrType>;  // Remove const for value_type
    using difference_type = std::ptrdiff_t;
    using pointer = PtrType*;
    using reference = PtrType&;

    explicit IteratorTemplate(pointer ptr) : ptr_(ptr) {}

    auto operator*() const -> reference { return *ptr_; }

    auto operator->() const -> pointer { return ptr_; }

    auto operator++() noexcept -> IteratorTemplate& {
      ptr_++;
      return *this;
    }

    auto operator++(int) noexcept -> IteratorTemplate {
      IteratorTemplate tmp = *this;
      ++(*this);
      return tmp;
    }

    friend auto operator==(const IteratorTemplate& a, const IteratorTemplate& b) noexcept -> bool {
      return a.ptr_ == b.ptr_;
    }

    friend auto operator!=(const IteratorTemplate& a, const IteratorTemplate& b) noexcept -> bool {
      return a.ptr_ != b.ptr_;
    }

   private:
    pointer ptr_;
  };

  using Iterator = IteratorTemplate<T>;
  using ConstIterator = IteratorTemplate<const T>;

  auto begin() const -> ConstIterator { return ConstIterator(&data_[0]); }

  auto end() const -> ConstIterator { return ConstIterator(&data_[size_]); }

  auto begin() -> Iterator { return Iterator(&data_[0]); }

  auto end() -> Iterator { return Iterator(&data_[size_]); }
};

}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_CXSTRUCTS_STACKSTACK_H_
