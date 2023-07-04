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

#ifndef CXSTRUCTS_SRC_BENCHMARK_H_
#define CXSTRUCTS_SRC_BENCHMARK_H_

#include <list>
#include <queue>
#include <random>
#include <unordered_set>
#include "CXStructs.h"
#include "CXTests.h"
#include "Comparisons.h"

//benchmarks are something close to this
static void VEC() {
  volatile int num1;
  volatile bool force;
  std::random_device rd;
  std::mt19937 gen(rd());

  //std::list<Data> vec;
  //cxstructs::LinkedList<Data> vec;
  cxstructs::HashMap<int, Data> vec;
  // std::unordered_map<int,Data>vec;
  //cxalgos::Queue<Data>vec;
  //std::queue<Data> vec;
  //std::unordered_set<int> vec;
  //cxstructs::HashSet<int> vec;
  //cxstructs::Queue<Data>vec;

  std::uniform_int_distribution<> distr(0, 100000);

  cxutil::now();
  for (uint_fast32_t i = 0; i < 1000000; i++) {
    force = vec.contains(i);
    vec.insert(distr(gen), Data());
    force = vec.contains(i);
  }
  for (uint_fast32_t i = 0; i < 1000000; i++) {
    force = vec.contains(i);
    vec.erase(distr(gen));
    force = vec.contains(i);
  }
  for (uint_fast32_t i = 0; i < 1000000; i++) {
    force = vec.contains(i);
    vec.insert(distr(gen), Data());
    force = vec.contains(i);
  }
  vec.clear();
  cxutil::printTime<>("cxstructs::vec");
}
#endif  //CXSTRUCTS_SRC_BENCHMARK_H_
