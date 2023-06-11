// Copyright (c) <2023> <gk646>
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

//
// Created by gk646 on 11.06.2023.
//

#include <iostream>

#include "datastructures/HashMap.h"
#include <unordered_map>

void compareHashMaps(){
  // Create instances
  HashMap<int, int> myMap;
  std::unordered_map<int, int> stdMap;

  // For time measurement
  std::clock_t start;
  double duration;

  int outerCount = 1000000;
  int innerCount = 100;

  // Test HashMap implementation
  start = std::clock();

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

  auto duration1 = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  std::cout << "HashMap: " << duration1 << " seconds" << '\n';

  // Test std::unordered_map
  start = std::clock();

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

  auto duration2 = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  std::cout << "std::unordered_map: " << duration2 << " seconds" << '\n';

  std::cout << duration1/duration2<< std::endl;
}