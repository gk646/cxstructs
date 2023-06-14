// Copyright (c) <2023> <gk646>
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
#define F
#ifndef CMAKE_TEST_BINARYTREE_H
#define CMAKE_TEST_BINARYTREE_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <stack>

/**
 * Simple BinaryTree implementation
 * Only compatible with comparable types "<"
 * @tparam T - type
 */
namespace cxstruct {
    template<typename T>
    class TreeNode {
        TreeNode *left_ = nullptr;
        TreeNode *right_ = nullptr;
        T data_;
    public:
        explicit TreeNode(T val) : data_(val), left_(nullptr), right_(nullptr) {}

        TreeNode(const TreeNode<T> &o)
                : data_(o.data_), left_(o.left_ ? new TreeNode(*o.left_) : nullptr),
                  right_(o.right_ ? new TreeNode(*o.right_) : nullptr) {}
    };

    template<typename T>
    class BinaryTree {
        TreeNode<T> *root_;
        uint_fast32_t size_ = 0;

        bool deleteNode(BinaryTree *parent, T val, bool left) {
            if (!root_->hasData_)
                return false;

            if (val < data_) {
                if (left_) {
                    left_->deleteNode(this, val, true);
                }
                return false;
            } else if (val > data_) {
                if (right_) {
                    right_->deleteNode(this, val, false);
                }
                return false;
            } else {
                if (!left_ && !right_) {
                    if (left) {
                        parent->left_ = nullptr;
                    } else {
                        parent->right_ = nullptr;
                    }
                    delete this;
                } else if (!left_ && !left) {
                    parent->right_ = this->right_;
                    delete this;
                } else if (!right_ && left) {
                    parent->left_ = this->left_;
                    delete this;
                } else {
                    throw std::logic_error("unsupported deletion");
                }
            }
        }

        int subTreeDepth(BinaryTree *node) {
            if (node == nullptr) {
                return 0;
            } else {
                int left = subTreeDepth(node->left_);
                int right = subTreeDepth(node->right_);
                return std::max(left, right) + 1;
            }
        }

        void insert(T val, TreeNode<T> *node) {
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

        bool contains() {

        }

    public:
        BinaryTree() = default;

        explicit BinaryTree(T data) : root_(new TreeNode<T>(data)) {};

        BinaryTree(const BinaryTree &o) = delete;

        BinaryTree &operator=(const BinaryTree &) = delete;

        ~BinaryTree() {
            delete root_;
        }

        /**
         * Inverts this BinaryTree starting from the given node
         */
        void invert(TreeNode<T> *node) {
            if (node == nullptr) return;
            std::swap(node->left, node->right);
            invert(node->left);
            invert(node->right);
        }

        /**
         * Inverts this BinaryTree starting from the root
         */
        void invert() {
            invert(root_);
        }

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
        bool contains(T val) {
            if (hasData_) {
                if (val < data_) {
                    if (left_) {
                        return left_->contains(val);
                    }
                    return false;
                } else if (val > data_) {
                    if (right_) {
                        return right_->contains(val);
                    }
                } else {
                    return true;
                }
            }
            return false;
        }

        /**
         * Erases the first occurrence of a node with this value
         * @param val - the node-value to search for
         * @return true if a deletion happened
         */
        bool erase(T val) {
            if (!hasData_)
                return false;

            if (data_ == val) {
                throw std::logic_error("unsupported");
            } else {
                if (val < data_ && left_) {
                    left_->deleteNode(this, val, true);
                } else if (val > data_ && right_) {
                    right_->deleteNode(this, val, false);
                } else {
                    return false;
                }
            }
        }

        /**
         * @return the total amount of nodes in the tree
         */
        [[nodiscard]] uint_fast32_t size() const { return size_; }

        [[nodiscard]] bool empty() const {
            return size_ == 0;
        }

        /**
         *
         * @return the maximum depth of the tree
         */
        uint_fast32_t maxDepth() { return subTreeDepth(this); }


        class InOrderIterator {
            std::stack<TreeNode<T> *> nodes;
        public:
            InOrderIterator(TreeNode<T> *root) {
                pushLeft(root);

            }

            void pushLeft(TreeNode<T> *node) {
                while (node) {
                    nodes.push(node);
                    node = node->left_;
                }
            }

            T &operator*() {
                return nodes.top().data_;

            }

            InOrderIterator &operator++() {
                TreeNode<T> *node = nodes.top();
                nodes.pop();
                pushLeft(node->right);
                return *this;
            }

            bool operator!=(const InOrderIterator &other) {
                return !nodes.empty();
            }
        };

        static void TEST() {
            BinaryTree<int> tree;
            tree.insert(10);
            assert(tree.contains(10));
            tree.insert(15);
            assert(tree.contains(15));
            tree.insert(5);
            assert(!tree.contains(4));

            tree.erase(10);
            assert(!tree.contains(10));
            tree.erase(5);
            assert(!tree.contains(5));


            tree.insert(10);
            tree.insert(15);
            tree.insert(5);

            //std::string normalTraversal = tree.inOrderTraversal();

            tree.invert();
            //std::string invertedTraversal = tree.inOrderTraversal();

            //assert(normalTraversal != invertedTraversal);
        }
    };
}
#endif // CMAKE_TEST_BINARYTREE_H
