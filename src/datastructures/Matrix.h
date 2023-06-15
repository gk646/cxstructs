// Copyright (c) 2023 gk646
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#define FINISHED
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <iostream>
/**
 * The usual normal 2d matrix implementation with a flattened array for more
 * cache efficieny
 */
namespace cxstructs {
class mat {
  float *arr;
  uint_fast16_t n_rows;
  uint_fast16_t n_cols;

public:
  mat() : n_cols(0), n_rows(0), arr(nullptr){};
  mat(uint_fast16_t rows, uint_fast16_t cols) : n_rows(rows), n_cols(cols) {
    arr = new float[n_cols * n_cols];
    for (int i = 0; i < n_cols * n_cols; i++) {
      arr[i] = 0;
    }
  }
  explicit mat(std::vector<std::vector<float>> vec)
      : n_rows(vec.size()), n_cols(vec[0].size()) {
    arr = new float[n_rows * n_cols];
    for (int i = 0; i < n_rows; i++) {
      std::copy_n(vec[i].begin(), n_cols, arr + i * n_cols);
    }
  }
  mat(const mat &o) : n_rows(o.n_rows), n_cols(o.n_cols) {
    arr = new float[n_rows * n_cols];
    std::copy(o.arr, o.arr + n_rows * n_cols, arr);
  }
  mat &operator=(const mat &other) {
    if (this != &other) {
      delete[] arr;

      n_rows = other.n_rows;
      n_cols = other.n_cols;

      arr = new float[n_rows * n_cols];
      std::copy(other.arr, other.arr + n_rows * n_cols, arr);
    }
    return *this;
  }
  ~mat() { delete[] arr; };
  mat operator*(const mat &b) {
    if (n_cols != b.n_rows) {
      throw std::invalid_argument(
          "first matrix n_rows doesnt match second matrix columns!");
    }

    mat result(n_rows, b.n_cols);

    for (uint_fast16_t i = 0; i < n_rows; i++) {
      for (uint_fast16_t j = 0; j < b.n_cols; j++) {
        float sum = 0.0f;
        for (uint_fast16_t k = 0; k < n_cols; k++) {
          sum += arr[i * n_cols + k] * b.arr[k * b.n_cols + j];
        }
        result.arr[i * result.n_cols + j] = sum;
      }
    }
    return result;
  }

  float &operator()(uint_fast32_t row, uint_fast32_t col) const {
    return arr[row * n_cols + col];
  }
  bool operator==(const mat &o) const;
  [[nodiscard]] mat transpose() const;
  mat operator+(const mat &o);
  [[nodiscard]] float dotProduct(const mat &o) const;
  void scalar(const float &s);

  friend std::ostream &operator<<(std::ostream &os, const mat &obj) {
    for (int i = 0; i < obj.n_rows; i++) {
      os << "[";
      for (int j = 0; j < obj.n_cols; j++) {
        os << obj.arr[i * obj.n_cols + j];
        if (j != obj.n_cols - 1) {
          os << ", ";
        }
      }
      os << "]";
      if (i != obj.n_rows - 1) {
        os << "\n";
      }
    }
    return os << std::endl;
  }

  [[nodiscard]] uint_fast32_t getRows() const { return n_rows; };
  [[nodiscard]] uint_fast32_t getCols() const { return n_cols; };

  static void TEST() {
    std::cout<< "MATRIX TESTS" << std::endl;
    // Test default constructor
    std::cout << "  Testing default constructor...\n";
    mat m1;
    assert(m1.arr == nullptr && m1.n_rows == 0 && m1.n_cols == 0);

    // Test parameterized constructor
    std::cout << "  Testing parameterized constructor...\n";
    mat m2(2, 3);
    assert(m2.n_rows == 2 && m2.n_cols == 3);

    // Test copy constructor
    std::cout << "  Testing copy constructor...\n";
    mat m3 = m2;
    assert(m3.n_rows == m2.n_rows && m3.n_cols == m2.n_cols);

    // Test multiplication and assignment operators
    std::cout << "  Testing multiplication and assignment operators...\n";
    mat m4(3, 2);
    m2 = m2 * m4;
    assert(m2.n_rows == 2 && m2.n_cols == 2);

    // Test constructor from vector of vectors
    std::cout << "  Testing constructor from vector of vectors...\n";
    std::vector<std::vector<float>> data = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    mat m5(data);
    assert(m5.n_rows == 2 && m5.n_cols == 2);
    assert(std::fabs(m5(0, 0) - 1.0f) < 1e-6);  // Comparing floats with a tolerance

    // Test specific multiplication
    std::cout << "  Testing specific multiplication...\n";
    std::vector<std::vector<float>> data1 = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    std::vector<std::vector<float>> data2 = {{5.0f, 6.0f}, {7.0f, 8.0f}};
    mat m6(data1);
    mat m7(data2);
    mat m8 = m6 * m7;
    assert(std::fabs(m8(0, 0) - 19.0f) < 1e-6);
    assert(std::fabs(m8(0, 1) - 22.0f) < 1e-6);
    assert(std::fabs(m8(1, 0) - 43.0f) < 1e-6);
    assert(std::fabs(m8(1, 1) - 50.0f) < 1e-6);
  }
};
} // namespace cxstructs