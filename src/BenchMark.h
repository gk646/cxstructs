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
#include <unordered_map>
#include <unordered_set>
#include "CXStructs.h"
inline static volatile int num1 = 2;
//benchmarks are this /  run separately
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

  cxstructs::now();
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
  cxstructs::printTime<>("cxstructs::vec");
}
static void UNORDERED_MAP() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, 100000);
  volatile int num1;
  now();
  std::unordered_map<int, Data> map;
  for (uint_fast32_t i = 0; i < 10; i++) {
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.insert({distr(gen), Data()});
    }
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.erase(j);
    }
    for (uint_fast32_t j = 0; j < 10000; j++) {
      num1 = map[j].num;
    }
  }
  map.clear();
  for (uint_fast32_t i = 0; i < 100; i++) {
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.insert({distr(gen), Data()});
    }
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.erase(j);
    }
  }
  printTime("std::unordered_map");
}
static void HASH_MAP() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, 100000);
  volatile int num1;
  now();
  HashMap<int, Data> map;
  for (uint_fast32_t i = 0; i < 10; i++) {
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.insert(distr(gen), Data());
    }
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.erase(i);
    }
    for (uint_fast32_t j = 0; j < 10000; j++) {
      num1 = map[j].num;
    }
  }
  map.clear();
  for (uint_fast32_t i = 0; i < 100; i++) {
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.insert(distr(gen), Data());
    }
    for (uint_fast32_t j = 0; j < 10000; j++) {
      map.erase(j);
    }
  }
  printTime("cxstructs::HashMap");
}

static void CX_QUEUE() {
  Queue<Data> q;
  now();
  for (uint_fast32_t i = 0; i < 1000; i++) {
    for (uint_fast32_t j = 0; j < 1000; j++) {
      q.emplace();
    }
    for (uint_fast32_t j = 0; j < 1000; j++) {
      num1 = q.back().num;
      num1 = q.front().num;
      q.pop();
      if (j == 999) {
        break;
      }
    }
    for (uint_fast32_t j = 0; j < 1000; j++) {
      q.emplace();
    }
    for (uint_fast32_t j = 0; j < 1000; j++) {
      num1 = q.back().num;
      num1 = q.front().num;
      q.pop();
    }
    num1 = q.size();
  }
  printTime("cxstructs::Queue");
}
static void STD_QUEUE() {
  std::queue<Data> q;
  now();
  for (uint_fast32_t i = 0; i < 1000; i++) {
    for (uint_fast32_t j = 0; j < 1000; j++) {
      q.emplace();
    }
    for (uint_fast32_t j = 0; j < 1000; j++) {
      num1 = q.back().num;
      num1 = q.front().num;
      q.pop();
      if (j == 999) {
        break;
      }
    }
    for (uint_fast32_t j = 0; j < 1000; j++) {
      q.emplace();
    }
    for (uint_fast32_t j = 0; j < 1000; j++) {
      num1 = q.back().num;
      num1 = q.front().num;
      q.pop();
    }
    num1 = q.size();
  }
  printTime("std::Queue");
}

#endif  //CXSTRUCTS_SRC_BENCHMARK_H_
