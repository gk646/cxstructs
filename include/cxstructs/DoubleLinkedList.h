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
#ifndef CXSTRUCTURES_DOUBLELINKEDLIST_H
#define CXSTRUCTURES_DOUBLELINKEDLIST_H

#include <algorithm>

#include <iostream>
#include "../cxconfig.h"

namespace cxhelper {
template <typename T>
struct DListNode {
  T val_;
  DListNode* next_;
  DListNode* previous_;
  explicit DListNode(const T& val)
      : val_(val), next_(nullptr), previous_(nullptr){};
  explicit DListNode(const T& val, DListNode<T>* previous)
      : val_(val), next_(nullptr), previous_(previous){};
};
}  // namespace cxhelper

namespace cxstructs {
using namespace cxhelper;

template <typename T>
class DoubleLinkedList {
  DListNode<T>* head_;
  DListNode<T>* end_;

  uint_32_cx size_;

 public:
  DoubleLinkedList() : size_(0), head_(nullptr), end_(nullptr){};
  DoubleLinkedList(const DoubleLinkedList<T>& o) : size_(o.size_) {
    if (o.head_) {
      head_ = new DListNode<T>(o.head_->val_, nullptr);

      DListNode<T>* current_new = head_;
      DListNode<T>* current_old = o.head_->next_;

      while (current_old != nullptr) {
        current_new->next_ = new DListNode<T>(current_old->val_, current_new);

        current_new = current_new->next_;
        current_old = current_old->next_;
      }
      end_ = current_new;
    } else {
      head_ = end_ = nullptr;
    }
  }
  DoubleLinkedList& operator=(const DoubleLinkedList<T>& o) {
    if (this == &o) {
      return *this;
    }

    clear();
    size_ = o.size_;

    if (o.head_) {
      head_ = new DListNode<T>(o.head_->val_, nullptr);

      DListNode<T>* current_new = head_;
      DListNode<T>* current_old = o.head_->next_;

      while (current_old != nullptr) {
        current_new->next_ = new DListNode<T>(current_old->val_, current_new);

        current_new = current_new->next_;
        current_old = current_old->next_;
      }
      end_ = current_new;
    } else {
      delete head_;
      delete end_;
      head_ = end_ = nullptr;
    }
    return *this;
  }
  ~DoubleLinkedList() {
    DListNode<T>* current = head_;
    while (current != nullptr) {
      DListNode<T>* next = current->next_;
      delete current;
      current = next;
    }
  }
  /** Adds a new element to the end of the list
  * @param val - the element to be added
   */
  void add(T val) {
    if (!head_) {
      head_ = new DListNode<T>(val);
      end_ = head_;
    } else {
      end_->next_ = new DListNode<T>(val, end_);
      end_ = end_->next_;
    }
    size_++;
  }
  /**
   * Removes the first node with the given value
  * @param val - the value to be matched
  */
  void clear() {
    DListNode<T>* current = head_;
    while (current != nullptr) {
      DListNode<T>* next = current->next_;
      delete current;
      current = next;
    }
    head_ = nullptr;
    end_ = nullptr;
    size_ = 0;
  }
  /**
    * @return the current n_elem of this Linked List
    */
  [[nodiscard]] uint_32_cx size() const { return size_; }
  /**
  * Removes the element at index counting from the start node
  * @param index  - the index at which to erase the element
  * @return the element removed with this operation
  */
  T removeAt(uint_32_cx index) {
    if (index >= size_)
      throw std::out_of_range("index out of bounds");

    DListNode<T>* toDelete;
    T val;
    if (index == 0) {
      toDelete = head_;
      head_ = head_->next_;
      if (head_) {
        head_->previous_ = nullptr;
      } else {
        delete head_;
        end_ = nullptr;
      }
    } else {
      DListNode<T>* it = head_;
      for (uint_32_cx i = 0; i < index; i++) {
        it = it->next_;
      }

      toDelete = it;
      if (it->next_) {
        it->next_->previous_ = it->previous_;
      } else {
        end_ = it->previous_;
      }
      if (it->previous_) {
        it->previous_->next_ = it->next_;
      }
    }

    val = toDelete->val_;
    delete toDelete;
    --size_;

    return val;
  }

  class Iterator {
   public:
    DListNode<T>* current;

    explicit Iterator(DListNode<T>* start) : current(start){};

    T& operator*() { return current->val_; }

    Iterator& operator++() {
      if (current) {
        current = current->next_;
      }
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return current == other.current;
    }

    bool operator!=(const Iterator& other) const {
      return current != other.current;
    }
  };

  Iterator begin() {
    if (size_ > 0) {
      return Iterator(head_);
    }
    return Iterator(nullptr);
  }
  Iterator end() { return Iterator(nullptr); }

  class ReverseIterator {
   public:
    DListNode<T>* current;

    explicit ReverseIterator(DListNode<T>* start) : current(start){};

    T& operator*() { return current->val_; }

    ReverseIterator& operator++() {
      if (current) {
        current = current->previous_;
      }
      return *this;
    }

    bool operator==(const ReverseIterator& other) const {
      return current == other.current;
    }

    bool operator!=(const ReverseIterator& other) const {
      return current != other.current;
    }
  };

  ReverseIterator begin_re() { return ReverseIterator(end_); }
  ReverseIterator end_re() { return ReverseIterator(nullptr); }
};
}  // namespace cxstructs

#endif  //CXSTRUCTURES_DOUBLELINKEDLIST_H
