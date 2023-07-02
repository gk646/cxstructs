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
#pragma warning(disable : 4834)

#include <iostream>
#include "CXStructs.h"

#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace cxstructs;
using namespace cxalgos;

struct Data {
  int arr[100]{};
  int num{};
  std::string string = "aösldkfjöalksdöfklj";
  Data() : num(0) {}
  Data(const Data& other) {
    std::copy_n(other.arr, 100, arr);
    num = other.num;
    string = other.string;
  }
  Data& operator=(const Data& other) {
    if (this != &other) {
      // Copy elements from other.arr to arr.
      std::copy_n(other.arr, 100, arr);
      // Copy other.num to num.
      num = other.num;
      // Copy other.string to string.
      string = other.string;
    }
    return *this;
  }
};
void compareWithSTL() {

  /* |-----------------------------------------------------|
   * |                       HASHMAPS                      |
   * |-----------------------------------------------------|
   */
  std::cout << "\nComapring HashMaps" << std::endl;
  HashMap<int, int> myMap;
  std::unordered_map<int, int> stdMap;

  int outerCount = 10000;
  int innerCount = 1000;

  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      myMap.insert(i, i * 2);
    }
    for (int i = 0; i < innerCount; i++) {
      myMap[i];
    }
    for (int i = 0; i < innerCount; i++) {
      myMap.erase(i);
    }
  }

  cxutil::printTime<>("cxstructs HashMap:");
  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      stdMap.insert({i, i * 2});
    }
    for (int i = 0; i < innerCount; i++) {
      stdMap[i];
    }
    for (int i = 0; i < innerCount; i++) {
      stdMap.erase(i);
    }
  }
  cxutil::printTime<>("std::unordered_map:");

  /* |-----------------------------------------------------|
   * |                 DYNAMIC ARRAYS                      |
   * |-----------------------------------------------------|
   */
  outerCount = 100;
  innerCount = 1000;

  std::cout << "\nComapring Dynamic Arrays" << std::endl;
  vec<Data> list;
  std::vector<Data> vector;

  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      list.emplace_back();
    }
    for (int i = 0; i < innerCount; i++) {
      list[i];
    }
    for (int i = 0; i < innerCount; i++) {
      list.removeAt(0);
    }
    for (const auto& num : list) {}
  }

  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      vector.emplace_back();
    }
    for (int i = 0; i < innerCount; i++) {
      vector[i];
    }
    for (int i = 0; i < innerCount; i++) {
      vector.erase(vector.begin());
    }
    for (const auto& num : vector) {}
  }
  cxutil::printTime<>("std::vector:");
  cxutil::printTime<>("cxstructs vec :");

  /* |-----------------------------------------------------|
   * |                       STACK                         |
   * |-----------------------------------------------------|
   */
  outerCount = 1000;
  innerCount = 10000;

  std::cout << "\nComapring Stacks" << std::endl;
  Stack<Data> stack;
  std::stack<Data> std_stack;
  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      stack.emplace();
    }
    for (int i = 0; i < innerCount; i++) {
      stack.top();
    }
    for (int i = 0; i < innerCount; i++) {
      stack.pop();
    }
  }
  cxutil::printTime<>("cxstructs stack :");
  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      std_stack.emplace();
    }
    for (int i = 0; i < innerCount; i++) {
      std_stack.top();
    }
    for (int i = 0; i < innerCount; i++) {
      std_stack.pop();
    }
  }
  cxutil::printTime<>("std::stack:");
  /* |-----------------------------------------------------|
   * |                     Queues                           |
   * |-----------------------------------------------------|
   */
  outerCount = 1000;
  innerCount = 10000;
  std::cout << "\nComapring Queues" << std::endl;
  Queue<Data> queue;
  std::queue<Data> std_queue;

  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      queue.emplace();
    }
    for (int i = 0; i < innerCount; i++) {
      queue.front();
    }
    for (int i = 0; i < innerCount; i++) {
      queue.pop();
    }
  }
  cxutil::printTime<>("cxstructs Queue :");

  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      std_queue.emplace();
    }
    for (int i = 0; i < innerCount; i++) {
      std_queue.front();
    }
    for (int i = 0; i < innerCount; i++) {
      std_queue.pop();
    }
  }
  cxutil::printTime<>("std::queue:");

  /* |-----------------------------------------------------|
   * |                        Trie                         |
   * |-----------------------------------------------------|
   */
  std::cout << "\nTesting Trie" << std::endl;
  // no STL counterpart

  Trie trie;

  cxutil::now();
  std::string nums;
  for (int i = 0; i < 100; i++) {
    trie.insert(nums += "a");
  }
  cxutil::printTime<std::chrono::milliseconds>("insert time:");

  cxutil::now();
  for (int i = 0; i < 10; i++) {
    trie.startsWith("a");
  }

  cxutil::printTime<std::chrono::milliseconds>("startsWith 1 time:");

  /* |-----------------------------------------------------|
   * |                      QUICKSORT                      |
   * |-----------------------------------------------------|
   */
  std::cout << "\nComparing Quicksort" << std::endl;

  auto vec1 = cxtests::generate_shuffled_vector(10000000);
  auto vec2 = vec1;
  cxutil::now();

  quick_sort(vec1.data(), 10000000);

  cxutil::printTime<>("cxalgos quicksort:");

  cxutil::now();

  std::sort(vec2.begin(), vec2.end());

  cxutil::printTime<>("std::sort:");

  /* |-----------------------------------------------------|
   * |                       DEQUEUE                       |
   * |-----------------------------------------------------|
   */

  std::cout << "\nComparing DeQueues" << std::endl;

  outerCount = 1000;
  innerCount = 1000;
  DeQueue<Data> de_queue;
  std::deque<Data> std_dequeue;

  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      de_queue.emplace_front();
    }
    for (int i = 0; i < innerCount; i++) {
      de_queue.front();
      de_queue.back();
    }
    for (int i = 0; i < innerCount; i++) {
      de_queue.pop_back();
    }
  }
  cxutil::printTime<>("cxstructs Queue :");
  cxutil::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      std_dequeue.emplace_front();
    }
    for (int i = 0; i < innerCount; i++) {
      std_dequeue.front();
      std_dequeue.back();
    }
    for (int i = 0; i < innerCount; i++) {
      std_dequeue.pop_back();
    }
  }
  cxutil::printTime<>("std::queue:");

  /* |-----------------------------------------------------|
   * |                         FNN                         |
   * |-----------------------------------------------------|
   */
  std::cout << "\nTiming FNN" << std::endl;
  const std::vector<std::vector<float>> inputs = {
      {1, 0}, {1, 1}, {0, 1}, {0, 0}};
  const std::vector<std::vector<float>> expected_outputs = {{1}, {0}, {1}, {0}};

  cxml::FNN fnn({2, 2, 1}, sig, 0.11);
  cxutil::now();
  for (int i = 0; i < 1000; ++i) {
    fnn.train(inputs, expected_outputs, 2000);
  }
  cxutil::printTime("FNN: 100 XOR trainings with 2000 iterations each:");

  /* |-----------------------------------------------------|
   * |                      MATRIX                         |
   * |-----------------------------------------------------|
   */

  std::cout << "\nTiming FNN" << std::endl;

  mat mat1{{2, 2}, {2, 2}};
  mat mat2{{4, 4}, {4, 4}};

  cxutil::now();
  for (int i = 0; i < 10000000; i++) {
    mat1* mat2;
  }
  cxutil::printTime("10 mil matrix 2x2 matrix multiplication");

  /* |-----------------------------------------------------|
   * |               PATTERN MATCHING                      |
   * |-----------------------------------------------------|
   */
  std::cout << "\nTiming Pattern Matching" << std::endl;

  std::string text;
  cxutil::load_txt("res/vulkan_doc.txt", text);

  cxutil::now();
  std::cout << findString_brute_force(text, "Yanjun") << std::endl;

  cxutil::printTime("13.9 mil characters searched in:");
}

void VEC() {}
