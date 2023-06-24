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

#include <cassert>

#include "datastructures/ArrayList.h"
#include "datastructures/BinaryTree.h"
#include "datastructures/DeQueue.h"
#include "datastructures/DoubleLinkedList.h"
#include "datastructures/FNN.h"
#include "datastructures/HashMap.h"
#include "datastructures/LinkedList.h"
#include "datastructures/Matrix.h"
#include "datastructures/Queue.h"
#include "datastructures/Stack.h"
#include "datastructures/Trie.h"

#include "algorithms/GraphTraversal.h"
#include "algorithms/Search.h"
#include "algorithms/Sorting.h"

namespace cxtests {
using namespace cxstructs;
using namespace cxalgos;

static void test_cxstructs() {

  mat::TEST();  // weird error in debug mode on the operator == for now reason
  LinkedList<int>::TEST();
  Queue<int>::TEST();
  HashMap<int, int>::TEST();  //potential memory leak
  Stack<int>::TEST();
  ArrayList<int>::TEST();  // potential memory leak
  Trie<>::TEST();
  DoubleLinkedList<int>::TEST();
  DeQueue<int>::TEST();
  FNN::TEST();
  //BinaryTree<int>::TEST();

  std::cout << "\nAll tests passed!" << std::endl;
}

static void test_cxalgos() {
  using namespace cxalgos;
  TEST_SORTING();
  TEST_DFS();
  TEST_SEARCH();
}

}  // namespace cxtests