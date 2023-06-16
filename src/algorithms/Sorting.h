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
#define F

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace cxalgos {
template <typename T> T *bubbleSort(T *arr, uint_fast32_t len, bool ascending) {
  if (ascending) {
    for (uint_fast32_t i = 1; i < len; i++) {
      for (uint_fast32_t j = 1; j < len; j++) {
        if (arr[i] < arr[i - 1]) {
          std::swap(arr[i], arr[i - 1]);
        }
      }
    }
  } else {
    for (uint_fast32_t i = 1; i < len; i++) {
      for (uint_fast32_t j = 1; j < len; j++) {
        if (arr[i] > arr[i - 1]) {
          std::swap(arr[i], arr[i - 1]);
        }
      }
    }
  }
  return arr;
}

template <typename T>
T *selectionSort(T *arr, uint_fast32_t len, bool ascending) {}

template <typename T> T *quickSort(T *arr, uint_fast32_t len, bool ascending) {}
template <typename T>
T *insertionSort(T *arr, uint_fast32_t len, bool ascending) {}
template <typename T> T *mergeSort(T *arr, uint_fast32_t len, bool ascending) {}
template <typename T> T *heapSort(T *arr, uint_fast32_t len, bool ascending) {}
namespace cxhelper {}
} // namespace cxalgos