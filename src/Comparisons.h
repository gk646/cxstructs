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

#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>
#include "cxtime.h"

#include "datastructures/BinaryTree.h"
#include "datastructures/DeQueue.h"
#include "datastructures/DoubleLinkedList.h"
#include "datastructures/HashMap.h"
#include "datastructures/LinkedList.h"
#include "datastructures/Queue.h"
#include "datastructures/Stack.h"
#include "datastructures/Trie.h"
#include "datastructures/vec.h"
#include "machinelearning/FNN.h"

#include "algorithms/GraphTraversal.h"
#include "algorithms/Search.h"
#include "algorithms/Sorting.h"

using namespace cxstructs;
using namespace cxalgos;
struct Data {
  int arr[100];
  int num;
  std::string string = "aösldkfjöalksdöfklj";
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
      myMap.remove(i);
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
  cxutil::printTime<>("cxstructs vec :");
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
      queue.emplace_back();
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
  trie.insert("hello");
  cxutil::printTime<std::chrono::nanoseconds>("insert time:");

  cxutil::now();
  trie.startsWith("hel");
  cxutil::printTime<std::chrono::nanoseconds>("startsWith 1 time:");

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
}
