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
#ifndef CXSTRUCTS_SRC_CXALGOS_STATISTIC_H_
#  define CXSTRUCTS_SRC_CXALGOS_STATISTIC_H_
#  include "../cxconfig.h"
#  include "Sorting.h"

namespace cxstructs {
/**
 * Returns the closest array value corresponding to the given quantile.
 * Copies the given array internally
 * @tparam T
 * @param quantile
 * @param arr
 * @param len
 * @return
 */
template <typename T>
inline T quantile_index(float quantile, const T* arr, int len) noexcept {
  if (len <= 0) {
    throw std::invalid_argument("Array length must be positive");
  }
  auto cpyArr = new T[len];
  memcpy_s(cpyArr, len, arr, len);
  cxstructs::quick_sort(cpyArr, len);
  float index = quantile * (float)len - 1;
  T val;
  if ((int)index - index < 0.000001) {
    val = cpyArr[(int)index];
  } else {
    val = cpyArr[(int)index + 1];
  }
  delete[] cpyArr;
  return val;
}

template <typename T>
inline float quartile_nth(uint8_t n, const T* arr, int len) noexcept {
  if (len <= 0) {
    throw std::invalid_argument("Array length must be positive");
  }
  auto cpyArr = new T[len];
  memcpy(cpyArr, arr, len * sizeof(int));
  cxstructs::selection_sort(cpyArr, len);

  float q = (len - 3) / 4.0F;
  q = n * q + n;

  T first = cpyArr[(int)q - 1];
  T second = cpyArr[(int)q];

  float factor = q - (int)q;
  float val = first + (second - first) * factor;

  delete[] cpyArr;
  return val;
}
}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_CXALGOS_STATISTIC_H_
