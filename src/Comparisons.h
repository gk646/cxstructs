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

#include <iostream>

#include <ctime>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>
#include "datastructures/HashMap.h"
#include "gktime.h"

using namespace cxstructs;
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
  std::cout << "COMPARING HASHMAPS\n" << std::endl;
  HashMap<int, int> myMap;
  std::unordered_map<int, int> stdMap;

  int outerCount = 10000;
  int innerCount = 1000;

  gkutils::now();
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

  gkutils::printTime<>("cxstructs HashMap:");
  gkutils::now();
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
  gkutils::printTime<>("std::unordered_map:");

  /* |-----------------------------------------------------|
   * |                 DYNAMIC ARRAYS                      |
   * |-----------------------------------------------------|
   */
  outerCount = 100000;
  innerCount = 100;

  std::cout << "COMPARING DYNAMIC ARRAYS\n" << std::endl;
  ArrayList<Data> list;
  std::vector<Data> vector;

  gkutils::now();
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
  gkutils::printTime<>("cxstructs ArrayList :");
  gkutils::now();
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
  gkutils::printTime<>("std::vector:");

  /* |-----------------------------------------------------|
   * |                     Queues                           |
   * |-----------------------------------------------------|
   */
  outerCount = 10000;
  innerCount = 100000;
  std::cout << "COMPARING QUEUES\n" << std::endl;
  Queue<int> queue;
  std::queue<int> std_queue;

  gkutils::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      queue.add(i * 2);
    }
    for (int i = 0; i < innerCount; i++) {
      queue.front();
    }
    for (int i = 0; i < innerCount; i++) {
      queue.pop();
    }
  }
  gkutils::printTime<>("cxstructs Queue :");
  gkutils::now();
  for (int k = 0; k < outerCount; k++) {
    for (int i = 0; i < innerCount; i++) {
      std_queue.emplace(i * 2);
    }
    for (int i = 0; i < innerCount; i++) {
      std_queue.front();
    }
    for (int i = 0; i < innerCount; i++) {
      std_queue.pop();
    }
  }
  gkutils::printTime<>("std::queue:");

  /* |-----------------------------------------------------|
   * |                        Trie                         |
   * |-----------------------------------------------------|
   */
  // no STL counterpart

  Trie trie;

  gkutils::now();

  trie.insert("hello");

  gkutils::printTime<std::chrono::nanoseconds>("insert time:");

  gkutils::now();

  trie.startsWith("hel");

  gkutils::printTime<std::chrono::nanoseconds>("startsWith 1 time:");
}
