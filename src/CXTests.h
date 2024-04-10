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

#include "cxstructs.h"
#include "cxml/word2vec.h"

#ifdef CX_INCLUDE_TESTS
namespace cxtests {

using namespace cxstructs;
static void test_cxstructs() {
  GEOMETRY_TEST();
  mat::TEST();
  LinkedList<int>::TEST();
  Queue<int>::TEST();
  Stack<int>::TEST();
  vec<int>::TEST();
  Trie::TEST();
  DoubleLinkedList<int>::TEST();
  DeQueue<int>::TEST();
  HashMap<int, int>::TEST();
  HashSet<int>::TEST();
  BinaryTree<int>::TEST();
  QuadTree<Point>::TEST();
  PriorityQueue<int>::TEST();
}

static void test_cxalgos() {
  TEST_SORTING();
  TEST_DFS();
  TEST_SEARCH();
  TEST_MATH();
  TEST_PATH_FINDING();
}

static void test_cxml() {
  FNN::TEST();
  kNN_2D<DataPoint_<float>>::TEST();
  Word2Vec::TEST();
}

static void test_all() {
  test_cxstructs();
  test_cxalgos();
  test_cxml();
  std::cout << "\nAll tests passed!" << std::endl;
}

}  // namespace cxtests
#endif
