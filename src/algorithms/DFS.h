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
#define FINISHED

#include <vector>
#include "../datastructures/BinaryTree.h"
#include <cassert>

#ifndef DSA_DFS_H
#define DSA_DFS_H


namespace cxalgo {
    using namespace cxstruct;

    /**
 * Treats the vector as an adjacency matrix
 * @return - the number of reachable nodes from the given start node
 */
    template<typename T>
    int DFS(std::vector<std::vector<T>> &mat, int nodeIndex, std::vector<bool> vis = {}) {
        int count = 1;
        if (mat.empty() || nodeIndex >= mat.size()) return 0;
        if (vis.empty()) {
            count++;
            vis = std::vector(mat.size(), false);
        }

        for (int i = 0; i < mat[nodeIndex].size(); ++i) {
            if (mat[nodeIndex][i] == 1 && !vis[i]) {
                vis[i] = true;
                count += DFS(mat, nodeIndex, vis);
            }
        }
        return count;
    }

    template<typename T>
    int DFS(BinaryTree<T> tree, T nodeValue, std::vector<bool> vis = {}) {
        int nums = 0;
        if (tree.empty()) return 0;
        if (vis.empty()) vis = std::vector<bool>(10, false);


    }

    static void TEST() {
        std::vector<std::vector<int>> mat = {
                {0, 1, 0, 1, 0},
                {1, 0, 1, 0, 0},
                {0, 1, 0, 0, 0},
                {1, 0, 0, 0, 1},
                {0, 0, 0, 1, 0}
        };

        // Perform DFS from node 0
        int reachableNodes = cxalgo::DFS(mat, 0);

        assert(reachableNodes == 5);
    }
}


#endif // DSA_DFS_H
