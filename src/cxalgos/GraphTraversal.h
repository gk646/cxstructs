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


#include <iostream>
#include <vector>
#include "../cxconfig.h"

#ifndef CXSTRUCTS_DFS_H
#define CXSTRUCTS_DFS_H

namespace cxalgos {

/**
 * Treats the vector as an adjacency matrix
 * @return - the number of reachable nodes from the given start node
 */
template <typename T>
int depth_first_search(std::vector<std::vector<T>>& mat, int nodeIndex,
                       std::vector<bool> vis = {}) {
  int count = 1;
  if (mat.empty() || nodeIndex >= mat.size())
    return 0;
  if (vis.empty()) {
    count++;
    vis = std::vector(mat.size(), false);
  }

  for (int i = 0; i < mat[nodeIndex].size(); ++i) {
    if (mat[nodeIndex][i] == 1 && !vis[i]) {
      vis[i] = true;
      count += depth_first_search(mat, nodeIndex, vis);
    }
  }
  return count;
}
}  // namespace cxalgos
#ifndef CX_DELETE_TESTS
namespace cxtests {  // namespace cxtests
using namespace cxalgos;
static void TEST_DFS() {

  std::cout << "TESTING DEPTH FIRST SEARCH" << std::endl;
  std::vector<std::vector<int>> mat = {{0, 1, 0, 1, 0},
                                       {1, 0, 1, 0, 0},
                                       {0, 1, 0, 0, 0},
                                       {1, 0, 0, 0, 1},
                                       {0, 0, 0, 1, 0}};

  // Perform depth_first_search from node 0
  int reachableNodes = depth_first_search(mat, 0);

  CX_ASSERT(reachableNodes == 5);
}
}  // namespace cxtests
#endif

#endif  // CXSTRUCTS_DFS_H
