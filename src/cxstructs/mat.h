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
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../cxconfig.h"
#include "vec.h"

namespace cxstructs {
/**
    <h2>2D Matrix</h2>
    This data structure is an efficient representation of a two-dimensional matrix, using a flattened array for cache efficiency and faster access times.
    <br><br>
    <b>Datatype:</b> The elements of the matrix are of type float.
    <br><br>
    <b>Indexing:</b> The matrix uses 0-based indexing.
    <br><br>
    <b>Initialization:</b> The default constructor initializes all elements to 0.
    <br><br>
    A 2D Matrix is essential in various applications such as graphics transformations, solving systems of linear equations, and data analysis. The flattened array representation ensures that the elements are stored in a contiguous block of memory, which is beneficial for cache performance.<p>

    <b>Use Cases:</b> 2D Matrices are widely used in linear algebra, image processing, computer graphics, and scientific computing.
    */
class mat {
  float* arr;
  uint_32_cx n_rows;
  uint_32_cx n_cols;

 public:
  inline mat() : n_cols(0), n_rows(0), arr(nullptr){};
  inline mat(std::initializer_list<float> list)
      : n_rows(1), n_cols(list.size()) {
    arr = new float[n_cols];
    uint32_t i = 0;
    for (float val : list) {
      arr[i++] = val;
    }
  }
  inline mat(std::initializer_list<std::initializer_list<float>> list)
      : n_rows(list.size()), n_cols(list.begin()->size()) {
    arr = new float[n_rows * n_cols];
    uint32_t i = 0;
    for (const auto& sublist : list) {
      for (float val : sublist) {
        arr[i++] = val;
      }
    }
  }
  /**
   * Constructs a matrix object with n_rows rows and n_cols columns <p>
   * Initialized with 0
   * @param rows number of rows
   * @param cols number of columns
   */
  inline mat(const uint_32_cx& n_rows, const uint_32_cx& n_cols)
      : n_rows(n_rows), n_cols(n_cols) {
    arr = new float[n_rows * n_cols];
    for (int i = 0; i < n_rows * n_cols; i++) {
      arr[i] = 0;
    }
  }
  inline explicit mat(std::vector<std::vector<float>> vec)
      : n_rows(vec.size()), n_cols(vec[0].size()) {
    arr = new float[n_rows * n_cols];
    for (int i = 0; i < n_rows; i++) {
      std::copy_n(vec[i].begin(), n_cols, arr + i * n_cols);
    }
  }
  /**
 * @brief Constructs a matrix with the specified number of rows and columns, and initializes its elements using a provided function.
 *
 * @tparam fill_form A callable object or function that takes a single int as an argument and returns a float
 * @param n_rows The number of rows in the matrix.
 * @param n_cols The number of columns in the matrix.
 * @param form The callable object or function used to initialize the elements of the matrix. It is invoked for each element with the element's index as an argument.
 * @note The `fill_form` function is invoked for each element of the matrix as `form(index)`, where `index` is the linear index of the element in row-major order (0-indexed).
 */
  template <typename fill_form,
            typename = std::enable_if_t<
                std::is_invocable_r_v<double, fill_form, double>>>
  inline mat(uint_32_cx n_rows, uint_32_cx n_cols, fill_form form)
      : n_rows(n_rows), n_cols(n_cols) {
    arr = new float[n_rows * n_cols];
    for (int i = 0; i < n_rows * n_cols; i++) {
      arr[i] = form(i);
    }
  }
  inline mat(const mat& o) : n_rows(o.n_rows), n_cols(o.n_cols) {
    arr = new float[n_rows * n_cols];
    std::copy(o.arr, o.arr + n_rows * n_cols, arr);
  }
  inline ~mat() { delete[] arr; };
  inline float& operator()(const uint_32_cx& row, const uint_32_cx& col) {
    return arr[row * n_cols + col];
  }
  /**
   * The underlying array is flattened!
   * A specific row and column can be accessed with:<p>
   * arr_[row * n_cols + column] <p>
   * Use with caution!
   * @return a pointer to the underlying array
   */
  float* get_raw() { return arr; }
  inline mat& operator=(const mat& other) {
    if (this != &other) {
      delete[] arr;

      n_rows = other.n_rows;
      n_cols = other.n_cols;

      arr = new float[n_rows * n_cols];
      std::copy(other.arr, other.arr + n_rows * n_cols, arr);
    }
    return *this;
  }
  /**
   * Returns a new matrix that is the result of the multiplication
   * of the current matrix with the provided matrix.
   * The number of columns in the first matrix must match the number
   * of rows in the second.
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator*(const mat& o) const {
    if (n_cols != o.n_rows) {
      throw std::invalid_argument(
          "first matrix n_cols doesnt match second matrix n_rows!");
    }

    mat result(n_rows, o.n_cols);
    for (uint_32_cx i = 0; i < n_rows; i++) {
      for (uint_32_cx j = 0; j < o.n_cols; j++) {
        float sum = 0.0f;
        for (uint_32_cx k = 0; k < n_cols; k++) {
          sum += arr[i * n_cols + k] * o.arr[k * o.n_cols + j];
        }
        result.arr[i * result.n_cols + j] = sum;
      }
    }
    return result;
  }
  /**
   * Returns a new matrix that
   * is the result of the addition of the current matrix with the provided matrix
   * The matrices must be of the same n_elem
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator+(const mat& o) const {
    if (o.n_cols != n_cols || o.n_rows != n_rows) {
      throw std::invalid_argument("invalid dimensions");
    }

    mat res(n_rows, n_cols);
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        res.arr[i * n_cols + j] = arr[i * n_cols + j] + o.arr[i * n_cols + j];
      }
    }
    return res;
  };
  /**
   * Returns a new matrix that is the
   * result of the subtraction of the provided matrix from the current matrix.
   * The matrices must be of the same n_elem
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator-(const mat& o) const {
    if (o.n_cols != n_cols || o.n_rows != n_rows) {
      throw std::invalid_argument("invalid dimensions");
    }

    mat res(n_rows, n_cols);
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        res.arr[i * n_cols + j] = arr[i * n_cols + j] - o.arr[i * n_cols + j];
      }
    }
    return res;
  };
  /**
   * Returns a new matrix that is the matrix Hadamard product (element-wise multiplication).
   * The matrices must be of the same n_elem
   * @param o other matrix
* @return the result of the operation
   */
  inline mat operator%(const mat& o) const {
    if (o.n_cols != n_cols || o.n_rows != n_rows) {
      throw std::invalid_argument("invalid dimensions");
    }

    mat res(n_rows, n_cols);
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        res.arr[i * n_cols + j] = arr[i * n_cols + j] * o.arr[i * n_cols + j];
      }
    }
    return res;
  }
  /**
   * Returns a new matrix that is the result of element-wise division of the current matrix by the provided matrix
   * The matrices must be of the same n_elem
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator/(const mat& o) const {
    if (o.n_cols != n_cols || o.n_rows != n_rows) {
      throw std::invalid_argument("invalid dimensions");
    }

    mat res(n_rows, n_cols);
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        res.arr[i * n_cols + j] = arr[i * n_cols + j] / o.arr[i * n_cols + j];
      }
    }
    return res;
  }
  /**
   * Checks whether the current matrix and the provided matrix are equal, i.e., have the same n_elem and identical elements in corresponding positions
   * @param o other matrix
   * @return true if the matrices are identical
   */
  inline bool operator==(const mat& o) const {
    if (this == &o) {
      return true;
    }
    if (o.n_cols == n_cols && o.n_rows == n_rows) {
      for (uint_32_cx i = 0; i < n_rows; i++) {
        for (uint_32_cx j = 0; j < n_cols; j++) {
          if (o.arr[i * n_cols + j] != arr[i * n_cols + j]) {
            return false;
          }
        }
      }
      return true;
    }
    return false;
  };
  /**
   * Checks whether the current matrix and the provided matrix are not equal.
   * @param o
   * @return true if the matrices are not identical
   */
  inline bool operator!=(const mat& o) const {
    if (this == &o) {
      return false;
    }
    if (o.n_cols == n_cols && o.n_rows == n_rows) {
      for (uint_32_cx i = 0; i < n_rows; i++) {
        for (uint_32_cx j = 0; j < n_cols; j++) {
          if (o.arr[i * n_cols + j] != arr[i * n_cols + j]) {
            return true;
          }
        }
      }
      return false;
    }
    return true;
  };
  /**
   * This method returns the transpose of the matrix. The transpose of
   * a matrix is a new matrix whose rows are the columns of the original matrix.
   * @return the transpose
   */
  [[nodiscard]] inline mat transpose() const {
    mat res(n_cols, n_rows);

    for (int i = 0; i < n_cols; i++) {
      for (int j = 0; j < n_rows; j++) {
        res.arr[i * n_rows + j] = arr[j * n_cols + i];
      }
    }
    return res;
  };
  /**
   * Allows you to perform a lambda function on all values of the given row
   * Both the current column and the value are given: col, val -> lambda()
   * @tparam lambda the function to perform
   * @param row row to perform the operation on
   * @param l the function to determine the new value
   */
  template <typename lambda>
  inline void row_op(uint_32_cx row, lambda l) {
    for (int i = 0; i < n_cols; i++) {
      arr[row * n_cols + i] = l(i, arr[row * n_cols + i]);
    }
  }
  /**
   * Allows you to perform a lambda function on all values of the given column
   * Both the current row and the value are given: row, val -> lambda(... return val)
   * @tparam lambda the function to perform
   * @param col the column to perform the operation on
   * @param l the function to determine the new value
   */
  template <typename lambda>
  inline void col_op(uint_32_cx col, lambda l) {
    for (int i = 0; i < n_rows; i++) {
      arr[i * n_cols + col] = l(i, arr[i * n_cols + col]);
    }
  }
  /**
 * Takes the dot product of each row of the matrix with the given vector and returns a new vector
 * @param vec
 * @return a new row with each index being the result of the vector-matrix dot product
 */
  [[nodiscard]] inline vec<float> dotProduct(const vec<float>& vec) const {
    cxstructs::vec<float> retval(n_rows, 0);
    for (int i = 0; i < n_rows; ++i) {
      for (int j = 0; j < n_cols; ++j) {
        retval[i] += arr[i * n_cols + j] * vec[j];
      }
    }
    return retval;
  }
  /**
   *  A scaled unit matrix is a matrix with dimensions n_rows and n_cols, with only one nonzero entry with value 1 at position M(row, col). <p>
   *
   * ij * M -> 0 except for the i-th row which is alpha * M[j, ]<p>
   * M * ij -> 0 except for the j-th column which is alpha * M[ ,i]
   *
   * @param n_rows the amount of row of the matrix unit
   * @param n_cols the amount of cols of the matrix unit
   * @param row row of 1
   * @param col col of 1
   * @return a new matrix with these properties
   */
  static inline mat unit_matrix(uint_32_cx n_rows, uint_32_cx n_cols,
                                uint_32_cx row, uint_32_cx col) {
    mat retval(n_rows, n_cols);
    retval(row, col) = 1;
    return retval;
  }
  /**
   * A scaled unit matrix is a matrix with dimensions n_rows and n_cols, with only one nonzero entry with value 1 * alpha at position M(row, col). <p>
   *
   * ij * M -> 0 except for the i-th row which is alpha * M[j, ]<p>
   * M * ij -> 0 except for the j-th column which is alpha * M[ ,i]
   * @param n_rows the amount of row of the matrix unit
   * @param n_cols the amount of cols of the matrix unit
   * @param row row of 1
   * @param col col of 1
   * @param alpha the scalar value
   * @return a new matrix with these properties
   */
  static inline mat unit_matrix_scaled(uint_32_cx n_rows, uint_32_cx n_cols,
                                       uint_32_cx row, uint_32_cx col,
                                       float alpha) {
    mat retval(n_rows, n_cols);
    retval(row, col) = 1 * alpha;
    return retval;
  }
  /**
   *A square matrix with ones on the main diagonal and zeros elsewhere.<p>
   * Has the property that when multiplied with any matrix M returns exactly M
   * @param size side length
   * @return a new identity matrix
   */
  static inline mat eye(uint_32_cx size) {
    mat eye(size, size);
    for (uint_fast32_t i = 0; i < size; i++) {
      eye.arr[i * size + i] = 1;
    }
    return eye;
  }
  /**
   * Multiplies the whole matrix with the given scalar in-place
   * @param s the scalar
   */
  inline void scale(const float& s) {
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        arr[i * n_cols + j] = arr[i * n_cols + j] * s;
      }
    }
  };
  /**Prints out the matrix
   * @param header optional header
   */
  void print(const std::string& header = "") const {
    if (!header.empty()) {
      std::cout << header << std::endl;
      for (int i = 0; i < n_rows; i++) {
        std::cout << "     [";
        for (int j = 0; j < n_cols; j++) {
          std::cout << arr[i * n_cols + j];
          if (j < n_cols - 1) {
            std::cout << ",";
          }
        }
        std::cout << "]\n";
      }
      return;
    }
    std::cout << *this << std::endl;
  }
  friend std::ostream& operator<<(std::ostream& os, const mat& obj) {
    for (int i = 0; i < obj.n_rows; i++) {
      os << "[";
      for (int j = 0; j < obj.n_cols; j++) {
        os << obj.arr[i * obj.n_cols + j];
        if (j != obj.n_cols - 1) {
          os << ", ";
        }
      }
      os << "]";
      os << "\n";
    }
    return os;
  }

  [[nodiscard]] inline uint_32_cx getRows() const { return n_rows; };
  [[nodiscard]] inline uint_32_cx getCols() const { return n_cols; };

#ifndef CX_DELETE_TESTS
  static void TEST() {
    std::cout << "MATRIX TESTS" << std::endl;
    // Test default constructor
    std::cout << "  Testing default constructor...\n";
    mat m1;
    CX_ASSERT(m1.arr == nullptr && m1.n_rows == 0 && m1.n_cols == 0);

    // Test parameterized constructor
    std::cout << "  Testing parameterized constructor...\n";
    mat m2(2, 3);
    CX_ASSERT(m2.n_rows == 2 && m2.n_cols == 3);

    // Test copy constructor
    std::cout << "  Testing copy constructor...\n";
    mat m3 = m2;
    CX_ASSERT(m3.n_rows == m2.n_rows && m3.n_cols == m2.n_cols);

    // Test multiplication and assignment operators
    std::cout << "  Testing multiplication and assignment operators...\n";
    mat m4(3, 2);
    m2 = m2 * m4;
    CX_ASSERT(m2.n_rows == 2 && m2.n_cols == 2);

    // Test constructor from vector of vectors
    std::cout << "  Testing constructor from vector of vectors...\n";
    std::vector<std::vector<float>> data = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    mat m5(data);
    CX_ASSERT(m5.n_rows == 2 && m5.n_cols == 2);
    CX_ASSERT(m5(0, 0) - 1.0f < 1e-6);  // Comparing floats with a tolerance

    // Test specific multiplication
    std::cout << "  Testing specific multiplication...\n";
    std::vector<std::vector<float>> data1 = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    std::vector<std::vector<float>> data2 = {{5.0f, 6.0f}, {7.0f, 8.0f}};
    mat m6(data1);
    mat m7(data2);
    mat m8 = m6 * m7;
    CX_ASSERT(m8(0, 0) - 19.0f < 1e-6);
    CX_ASSERT(m8(0, 1) - 22.0f < 1e-6);
    CX_ASSERT(m8(1, 0) - 43.0f < 1e-6);
    CX_ASSERT(m8(1, 1) - 50.0f < 1e-6);

    std::cout << "  Testing equals operator...\n";

    mat m9({{2, 3}, {2, 3}});
    mat m10({{2, 3}, {2, 3}});

    CX_ASSERT(m9 == m10);
    CX_ASSERT(!(m9 != m10));
    m9(1, 1) = 5;
    CX_ASSERT(m9 != m10);
    CX_ASSERT(!(m9 == m10));

    std::cout << "  Testing transpose...\n";
    m10 = mat(2, 3);
    m10(0, 0) = 1;
    m10(0, 1) = 2;
    m10(0, 2) = 3;
    m10(1, 0) = 4;
    m10(1, 1) = 5;
    m10(1, 2) = 6;

    mat m1_transpose = m10.transpose();

    CX_ASSERT(m1_transpose.n_rows == m10.n_cols);
    CX_ASSERT(m1_transpose.n_cols == m10.n_rows);
    CX_ASSERT(m1_transpose(0, 0) == m10(0, 0));
    CX_ASSERT(m1_transpose(0, 1) == m10(1, 0));
    CX_ASSERT(m1_transpose(1, 0) == m10(0, 1));
    CX_ASSERT(m1_transpose(2, 0) == m10(0, 2));
    CX_ASSERT(m1_transpose(2, 1) == m10(1, 2));

    std::cout << "  Testing assignment and equality...\n";
    mat m11(2, 2);
    m11(0, 0) = 1;
    m11(0, 1) = 2;
    m11(1, 0) = 3;
    m11(1, 1) = 4;

    mat m12 = m11;

    CX_ASSERT(m11 == m12);
    CX_ASSERT(!(m11 != m12));

    std::cout << "  Testing addition and subtraction...\n";
    mat m13(2, 2);
    m13(0, 0) = 1;
    m13(0, 1) = 2;
    m13(1, 0) = 3;
    m13(1, 1) = 4;

    mat m14(2, 2);
    m14(0, 0) = 5;
    m14(0, 1) = 6;
    m14(1, 0) = 7;
    m14(1, 1) = 8;

    mat m15 = m13 + m14;
    CX_ASSERT(m15(0, 0) == 6);
    CX_ASSERT(m15(0, 1) == 8);
    CX_ASSERT(m15(1, 0) == 10);
    CX_ASSERT(m15(1, 1) == 12);

    mat m16 = m15 - m14;
    CX_ASSERT(m16 == m13);

    std::cout << "  Testing multiplication...\n";

    mat m17(2, 2);
    m17(0, 0) = 1;
    m17(0, 1) = 2;
    m17(1, 0) = 3;
    m17(1, 1) = 4;

    mat m18(2, 2);
    m18(0, 0) = 5;
    m18(0, 1) = 6;
    m18(1, 0) = 7;
    m18(1, 1) = 8;

    mat m19 = m17 * m18;
    CX_ASSERT(m19(0, 0) == 19);
    CX_ASSERT(m19(0, 1) == 22);
    CX_ASSERT(m19(1, 0) == 43);
    CX_ASSERT(m19(1, 1) == 50);

    std::cout << "  Testing row and col operations...\n";

    mat m20(2, 2);
    m20(0, 0) = 1;
    m20(0, 1) = 2;
    m20(1, 0) = 3;
    m20(1, 1) = 4;

    m20.row_op(1, [](uint_32_cx col, float val) { return val * 2; });
    CX_ASSERT(m20(1, 0) == 6);
    CX_ASSERT(m20(1, 1) == 8);

    m20.col_op(0, [](uint_32_cx row, float val) { return val + 1; });
    CX_ASSERT(m20(0, 0) == 2);
    CX_ASSERT(m20(1, 0) == 7);

    std::cout << "  Testing print...\n";
    m20.print();
    m20.print("m20");
    mat mem_leak(20000, 20000);
  }
#endif
};
}  // namespace cxalgos