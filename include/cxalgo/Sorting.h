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

namespace cxhelper {  // helper methods to provide clean calling interface
template <typename T>
void quick_sort_internal_sort(T* arr, int_fast32_t low, int_fast32_t high) {
  if (low < high) {

    const T& pivot = arr[high];
    uint_fast32_t n = low;
    for (uint_fast32_t i = low; i < high; i++) {
      if (arr[i] <= pivot) {
        std::swap(arr[n], arr[i]);
        n++;
      }
    }

    std::swap(arr[n], arr[high]);

    quick_sort_internal_sort(arr, low, n - 1);
    quick_sort_internal_sort(arr, n + 1, high);
  }
}

}  // namespace cxhelper

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
void bubble_sort(T* arr, uint_fast32_t len, bool ascending = true) {
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
/**
 * Selection sort is also a simple in-place sorting algorithm. The idea is to search for the
 * smallest element in the remaining list and then swap it with your position. Then move one forward.
 * By repeating this n-times throughout the list we always swap in the smallest element giving us sorting in
 * ascending order.
 *<p> Best: O(n^2) <p> Average: O(n^2) <p> Worst: O(n^2)
 * @tparam T
 * @param arr
 * @param len
 * @param ascending
 */
template <typename T>
void selectionSort(T* arr, uint_fast32_t len, bool ascending = true) {
  uint_fast32_t index;
  if(ascending){
    for (int i = 0; i < len; i++) {
      T low = arr[i];
      index = i;
      for (int j = i + 1; j < len; j++) {
        if (arr[j] < low) {
          low = arr[j];
          index = j;
        }
      }
      std::swap(arr[i], arr[index]);
    }
  } else {
    for (int i = 0; i < len; i++) {
      T low = arr[i];
      index = i;
      for (int j = i + 1; j < len; j++) {
        if (arr[j] < low) {
          low = arr[j];
          index = j;
        }
      }
      std::swap(arr[i], arr[index]);
    }
  }
}
/**
 * <h2>Quicksort</h2>
 * is the most widely used sorting algorithm. To start you choose a pivot value from the array.
 * After this pivot value you rearrange all smaller values to its left and bigger values to its right.
 * <p>
 * This means that the pivot element is in its final state. Then you recursively apply the same step to both halves,
 * the first from 0 to pivot-1, the second from pivot+1 to end.
 * The partition step inherently makes it unstable. <p>
 * Best: O(n log n)<p>
 * Average: O(n log n)<p>
 * Worst: O(n^2)<p>
 *
 * @tparam T type
 * @param arr array to sort
 * @param len length of the array
 * @param ascending true to sort in ascending order, false to sort in descending order
 */

template <typename T>
void quick_sort(T* arr, uint_fast32_t len, bool ascending = true) {
  cxhelper::quick_sort_internal_sort(arr, 0, len - 1);
  if (!ascending) {
    std::reverse(arr, arr + len);
  }
}
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
using namespace cxalgos;

static void TEST_SORTING() {
  std::cout << "TESTING BUBBLESORT" << std::endl;
  int arr[] = {3, 1, 2, 5, 1, 4, 0, 1001, -10};
  int sorted[] = {-10, 0, 1, 1, 2, 3, 4, 5, 1001};
  bubble_sort(arr, 9);
  for (uint_fast32_t i = 0; i < 9; i++) {
    assert(arr[i] == sorted[i]);
  }
  int arr1[] = {3, 1, 2, 5, 1, 4, 0, 1001, -10};
  int sorted1[] = {-10, 0, 1, 1, 2, 3, 4, 5, 1001};

  quick_sort(arr1, 9);
  for (uint_fast32_t i = 0; i < 9; i++) {
    std::cout << arr1[i] << std::endl;
    assert(arr1[i] == sorted1[i]);
  }
  quick_sort(arr1, 9, false);
  for (uint_fast32_t i = 0; i < 9; i++) {

    assert(arr1[8 - i] == sorted1[i]);
  }

  int arr2[] = {3, 1, 2, 5, 1, 4, 0, 1001, -10};
  int sorted2[] = {-10, 0, 1, 1, 2, 3, 4, 5, 1001};

  selectionSort(arr2, 9);
  for (uint_fast32_t i = 0; i < 9; i++) {
    std::cout << arr2[i] << std::endl;
    assert(arr2[i] == sorted2[i]);
  }
}
}  // namespace cxtests

#endif  // CXSTRUCTS_SORTING_H
