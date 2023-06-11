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
// Created by Lukas on 01.06.2023.
//

#ifndef CMAKE_TEST_BINARYTREE_H
#define CMAKE_TEST_BINARYTREE_H

#include <algorithm>
#include <iostream>
#include <stdexcept>

template<typename T>
class BinaryTree {
    BinaryTree *left_ = nullptr;
    BinaryTree *right_ = nullptr;
    T data_;
    bool hasData_ = false;
    uint_fast16_t size_ = 0;

    bool deleteNode(BinaryTree *parent, T val, bool left) {
        if (!hasData_)
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

public:
    BinaryTree() = default;

    explicit BinaryTree(T data) : data_(data), hasData_(true) {};

    BinaryTree(const BinaryTree &o) = delete;

    BinaryTree &operator=(const BinaryTree &) = delete;

    ~BinaryTree() {
        delete left_;
        delete right_;
    }

    T get() {
        if (hasData_) {
            return data_;
        }
        throw std::out_of_range("no such element");
    }

    void insert(T val) {
        size_++;
        if (hasData_) {
            if (val < data_) {
                if (!left_) {
                    left_ = new BinaryTree(val);
                } else
                    left_->insert(val);
            } else {
                if (!right_) {
                    right_ = new BinaryTree(val);
                } else
                    right_->insert(val);
            }
        } else {
            data_ = val;
            hasData_ = true;
        }
    }

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

    int size() const { return size_; }

    int depth() { return subTreeDepth(this); }
};

#endif // CMAKE_TEST_BINARYTREE_H
