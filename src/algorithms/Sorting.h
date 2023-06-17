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
#ifndef CXSTRUCTS_SORTING_H
#define CXSTRUCTS_SORTING_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace cxalgos {
/**
 * Bubble sort is the one of the simplest sorting algorithms. It works by
 * checking to adjacent values and swapping them if they are not in the right
 * order. <p> Best: O(n^2) <p> Average: O(n^2) <p> Worst: O(n^2)
 * @tparam T type
 * @param arr array to sort
 * @param len  length of the array
 * @param ascending true to sort ascending, false to sort descending
 */
template <typename T>
void bubble_sort(T* arr, uint_fast32_t len, bool ascending) {
  if (ascending) {
    for (uint_fast32_t i = 0; i < len; i++) {
      for (uint_fast32_t j = 1; j < len; j++) {
        if (arr[j] < arr[j - 1]) {
          std::swap(arr[j], arr[j - 1]);
        }
      }
    }
  } else {
    for (uint_fast32_t i = 0; i < len; i++) {
      for (uint_fast32_t j = 1; j < len; j++) {
        if (arr[j] > arr[j - 1]) {
          std::swap(arr[j], arr[j - 1]);
        }
      }
    }
  }
}

template <typename T>
T* selectionSort(T* arr, uint_fast32_t len, bool ascending) {}
/**
 * Quick sort has the best possible O notation runtime for all cases but in
 * practice is often slower than for example quicksort. It works by dividing the
 * array into sub-arrays of decreasing length and sorting and merging them back
 * together.<p> Best: O(n) <p> Average: O(n*log(n)) <p> Worst: O(n^2)
 * @tparam T type
 * @param arr array to sort
 * @param len  length of the array
 * @param ascending true to sort ascending, false to sort descending
 */
template <typename T>
void quickSort(T* arr, uint_fast32_t len, bool ascending) {}
template <typename T>
T* insertionSort(T* arr, uint_fast32_t len, bool ascending) {}
/**
 * Merge sort has the best possible O notation runtime for all cases but in
 * practice is often slower than for example quicksort. It works by dividing the
 * array into sub-arrays of decreasing length to then sort and merge them back
 * together. <p> Best: O(n*log(n)) <p> Average: O(n*log(n)) <p> Worst: O(n*log(n))
 * @tparam T type
 * @param arr array to sort
 * @param len  length of the array
 * @param ascending true to sort ascending, false to sort descending
 */
template <typename T>
void mergeSort(T* arr, uint_fast32_t len, bool ascending) {}
template <typename T>
void heapSort(T* arr, uint_fast32_t len, bool ascending) {}

}  // namespace cxalgos

namespace cxtests {

static void TEST_ALGOS() {
  std::cout << "TESTING BUBBLESORT" << std::endl;
  int arr[] = {3, 1, 2, 5, 1, 4, 0, 1001, -10};
  int sorted[] = {-10, 0, 1, 1, 2, 3, 4, 5, 1001};
  bubble_sort(arr, 9, true);
  for (std::uint_fast32_t i = 0; i < 8; i++) {
    assert(arr[i] == sorted[i]);
  }
}
}  // namespace cxtests

#endif  // CXSTRUCTS_SORTING_H
