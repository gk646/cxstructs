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
#ifndef SMALLVECTOR_H
#define SMALLVECTOR_H

#include <cstdint>

namespace cxstructs {
template <typename T, uint32_t N = 16, typename size_type = uint16_t>
struct SmallVector {
  SmallVector() : m_size(0), m_capacity(N), m_data(m_stack_data) {}

  ~SmallVector() {
    if (m_data != m_stack_data) {
      delete[] m_data;
    }
  }

  void push_back(const T& value) {
    ensure_capacity(m_size + 1);
    m_data[m_size++] = value;
  }

  void pop_back() {
    assert(m_size > 0);
    --m_size;
  }

  // ensures capacity - initializes all new values
  void resize(size_type new_size, const T& val) {
    if (new_size > m_size) {
      ensure_capacity(new_size);
      for (size_type i = m_size; i < new_size; ++i) {
        m_data[i] = val;
      }
      m_size = new_size;
    }
  }

  T& operator[](size_type index) {
    assert(index < m_size);
    return m_data[index];
  }

  const T& operator[](size_type index) const {
    assert(index < m_size);
    return m_data[index];
  }

  size_type size() const { return m_size; }

  size_type capacity() const { return m_capacity; }

  const T* data() const { return m_data; }

  T* data() { return m_data; }

 private:
  void ensure_capacity(size_type new_capacity) {
    if (new_capacity <= m_capacity) {
      return;
    }

    new_capacity = m_capacity * 2;
    T* new_data = new T[new_capacity];

    std::memcpy(new_data, m_data, m_size * sizeof(T));

    if (m_data != m_stack_data) {
      delete[] m_data;
    }

    m_data = new_data;
    m_capacity = new_capacity;
  }
  size_type m_size;
  size_type m_capacity;
  T* m_data;
  T m_stack_data[N];
};
}  // namespace cxstructs
#endif  //SMALLVECTOR_H