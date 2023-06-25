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
#define F
#ifndef CXSTRUCTS_BINARYTREE_H
#define CXSTRUCTS_BINARYTREE_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <vector>
#include "../cxconfig.h"

namespace cxhelper {  // namespace to hide helper structs
template <typename T>
struct TreeNode {
  TreeNode* left_ = nullptr;
  TreeNode* right_ = nullptr;
  T data_;

  explicit TreeNode(T val) : data_(val), left_(nullptr), right_(nullptr) {}

  TreeNode(const TreeNode<T>& o)
      : data_(o.data_),
        left_(o.left_ ? new TreeNode(*o.left_) : nullptr),
        right_(o.right_ ? new TreeNode(*o.right_) : nullptr) {}
};
}  // namespace cxhelper
namespace cxstructs {
using namespace cxhelper;
/** <h2> Binary Tree.</h2>
 *
 * A Binary Tree is a fundamental data structure that uses a hierarchical model. Each element, known as a node,
 * has at most two children: <b>left child</b> and <b>right child</b>.
 *
 * The tree starts from the <b>root node</b>, and any node with no children is called a <b>leaf node</b>.
 *<br><br>
 * It is most useful for data that can be organized hierarchically. Each node should be a type (T) that supports
 * comparison using the '\<' operator, to enable sorting and searching operations.
 * <br><br>
 * While accessing or searching for a specific element in a binary tree requires traversing potentially half the tree,
 * a balanced binary tree allows for faster lookup times of O(log N).
 */
template <typename T>
class BinaryTree {
  TreeNode<T>* root_;
  uint_32_cx size_ = 0;

  int subTreeDepth(BinaryTree* node) {
    if (node == nullptr) {
      return 0;
    } else {
      int left = subTreeDepth(node->left_);
      int right = subTreeDepth(node->right_);
      return std::max(left, right) + 1;
    }
  }

  void insert(T val, TreeNode<T>* node) {
    if (node) {
      if (val < node->data_) {
        if (!node->left_) {
          node->left_ = new BinaryTree(val);
        } else
          node->left_->insert(val);
      } else {
        if (!node->right_) {
          node->right_ = new BinaryTree(val);
        } else
          node->right_->insert(val);
      }
    } else {
      root_ = new TreeNode<T>(val);
    }
  }

  bool contains(T val, TreeNode<T>* node) {
    if (node) {
      if (val < node->data_) {
        if (node->left_) {
          return node->left_->contains(val);
        }
        return false;
      } else if (val > node->data_) {
        if (node->right_) {
          return node->right_->contains(val);
        }
      } else {
        return true;
      }
    }
    return false;
  }

 public:
  BinaryTree() = default;
  explicit BinaryTree(T data) : root_(new TreeNode<T>(data)){};
  BinaryTree(const BinaryTree& o) = delete;
  BinaryTree& operator=(const BinaryTree&) = delete;
  ~BinaryTree() { delete root_; }
  /**
   * Gives access to the root node of the tree
   * @return the root node
   */
  TreeNode<T>* get_root() { return root_; }

  /**
   * Inverts this BinaryTree starting from the given node
   */
  void invert(TreeNode<T>* node) {
    if (node == nullptr)
      return;
    std::swap(node->left, node->right);
    invert(node->left);
    invert(node->right);
  }

  /**
   * Inverts this BinaryTree starting from the root
   */
  void invert() { invert(root_); }

  /**
   * Inserts the given element into the tree at the right position
   * @param val - the inserted value
   */
  void insert(T val) {
    if (root_) {
      insert(val, root_);
      size_++;
      return;
    }
    size_++;
    root_ = new TreeNode<T>(val);
  }

  /**
   * Recursively searches the tree for the given value
   * @param val - the value to search for
   * @return - true if the tree contains the given value, false otherwise
   */
  bool contains(T val) { return contains(val, root_); }

  /**
   * Erases the first occurrence of a node with this value
   * @param val - the node-value to search for
   * @return true if a deletion happened
   */
  bool erase(T val) {
    throw std::logic_error("missing implementation");
    /*
    if (!hasData_)
      return false;

    if (arr_ == val) {
      throw std::logic_error("unsupported");
    } else {
      if (val < arr_ && left_) {
        left_->deleteNode(this, val, true);
      } else if (val > arr_ && right_) {
        right_->deleteNode(this, val, false);
      } else {
        return false;
      }
    }
     */
  }

  /**
   * @return the total amount of nodes in the tree
   */
  [[nodiscard]] uint_32_cx size() const { return size_; }

  [[nodiscard]] bool empty() const { return size_ == 0; }

  /**
   *
   * @return the maximum depth of the tree
   */
  uint_32_cx maxDepth() { return subTreeDepth(this); }

  class InOrderIterator {
    std::stack<TreeNode<T>*> nodes;

   public:
    InOrderIterator(TreeNode<T>* root) { pushLeft(root); }

    void pushLeft(TreeNode<T>* node) {
      while (node) {
        nodes.push(node);
        node = node->left_;
      }
    }

    T& operator*() { return nodes.top().arr_; }

    InOrderIterator& operator++() {
      TreeNode<T>* node = nodes.top();
      nodes.pop();
      pushLeft(node->right);
      return *this;
    }

    bool operator!=(const InOrderIterator& other) { return !nodes.empty(); }
  };
  /**
   * In order tree traversal
   * @return a in order tree traversal begin iterator
   */
  InOrderIterator begin() { return InOrderIterator(root_); }
  /**
   * In order tree traversal
* @return a in order tree traversal end iterator

   */
  InOrderIterator end() { return InOrderIterator(nullptr); }
  static void TEST() {
    BinaryTree<int> tree;

    assert(tree.empty());      // Test empty on newly created tree
    assert(tree.size() == 0);  // Test n_elem on newly created tree

    tree.insert(10);
    assert(tree.contains(10));
    assert(tree.maxDepth() == 1);  // Test maxDepth after inserting one element
    assert(!tree.empty());         // Test empty after inserting one element
    assert(tree.size() == 1);      // Test n_elem after inserting one element

    tree.insert(15);
    assert(tree.contains(15));
    assert(tree.maxDepth() ==
           2);  // Test maxDepth after inserting second element

    tree.insert(5);
    assert(tree.contains(5));
    assert(!tree.contains(4));
    assert(tree.maxDepth() ==
           2);  // Test maxDepth after inserting third element

    std::vector<int> normalTraversal, invertedTraversal;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
      normalTraversal.push_back(*it);
    }

    tree.invert();

    for (auto it = tree.begin(); it != tree.end(); ++it) {
      invertedTraversal.push_back(*it);
    }

    assert(
        normalTraversal !=
        invertedTraversal);  // Assert that normal and inverted traversals are
                             // different

    std::cout << "All tests passed!" << std::endl;
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_BINARYTREE_H
