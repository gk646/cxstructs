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
#ifndef CXSTRUCTS_SRC_DATASTRUCTURES_ROWVECTOR_H_
#define CXSTRUCTS_SRC_DATASTRUCTURES_ROWVECTOR_H_

#include <algorithm>
#include <cstdint>

namespace cxstructs {
template <typename T>
class row_vec {
  uint_fast32_t size_;
  T* data_;

 public:
  row_vec() : size_(0), data_(nullptr) {}
  explicit row_vec(uint_fast32_t size) : size_(size), data_(new T[size]) {}
  row_vec(uint_fast32_t size, const T val) : size_(size), data_(new T[size]) {
    std::fill(data_, data_ + size, val);
  }

  [[nodiscard]] inline T& operator[](uint_fast32_t i) const { return data_[i]; }
};
}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_ROWVECTOR_H_
