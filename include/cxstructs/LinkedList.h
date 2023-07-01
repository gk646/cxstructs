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
#ifndef CXSTRUCTS_LINKEDLIST_H
#define CXSTRUCTS_LINKEDLIST_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "../cxconfig.h"

namespace cxhelper {
template <typename T>
struct ListNode {
  T val_;
  ListNode* next_;

  inline ListNode(const ListNode& o) : val_(o.val_), next_(nullptr) {}
  inline ListNode(const T& val, ListNode* next) : val_(val), next_(next) {}
  inline explicit ListNode(const T& val) : val_(val), next_(nullptr) {}
  template <typename... Args>
  inline explicit ListNode(Args&&... args)
      : val_(std::forward<Args>(args)...), next_(nullptr) {}
  [[nodiscard]] inline T& get() { return val_; }
};

}  // namespace cxhelper
namespace cxstructs {
using namespace cxhelper;
/**
 * <h2>Singly Linked List</h2>
 *
 * is a fundamental data structure that consists of a sequence of nodes, where each node contains
 * data and a reference (or link) to the next node in the sequence.
 * <br><br>
 * Adding nodes to the list is a <b>highly efficient O(1) operation </b, as it only requires
 * updating the end of the list and the link of a single node.
 * <br><br>
 * However, accessing or searching for specific elements in the list requires potentially <b> traversing the entire list,
 * which is an O(n)</b> operation. This makes it less suitable for cases where random access is frequently required.<p>
 */
template <typename T>
class LinkedList {
  using Node = ListNode<T>;

  uint_32_cx size_;
  Node sentinel_;
  Node* end_;
  //Node* prev_end_; // to allow for quick pop_back()

 public:
  LinkedList() : sentinel_(T()), end_(&sentinel_), size_(0){};
  LinkedList(const LinkedList<T>& o)
      : sentinel_(T()), end_(&sentinel_), size_(0) {
    Node* current_old = o.sentinel_.next_;
    while (current_old != nullptr) {
      push_back(current_old->val_);
      current_old = current_old->next_;
    }
  }
  LinkedList& operator=(const LinkedList<T>& o) {
    if (this != &o) {
      clear();
      Node* current_old = o.sentinel_.next_;
      while (current_old != nullptr) {
        push_back(current_old->val_);
        current_old = current_old->next_;
      }
    }
    return *this;
  }
  ~LinkedList() {
    Node* current = sentinel_.next_;
    while (current != nullptr) {
      Node* next = current->next_;
      delete current;
      current = next;
    }
  }
  /**
* Adds a new element to the end of the list
* @param val - the element to be added
*/
  inline void push_back(const T& val) {
    end_->next_ = new Node(val);
    end_ = end_->next_;
    size_++;
  }
  template <typename... Args>
  inline void emplace_back(Args&&... args) {
    end_->next_ = new Node(std::forward<Args>(args)...);
    end_ = end_->next_;
    size_++;
  }
  /**
  * Removes the element at index counting from the start node
 * @param index  - the index at which to erase the element
 * @return the element removed with this operation
  */
  inline T erase_at(const uint_32_cx& index) {
    CX_ASSERT(index < size_ && "index too big");

    Node* toDelete;
    T val;
    if (index == 0) {
      toDelete = sentinel_.next_;
      sentinel_.next_ = toDelete->next_;
      if (size_ == 1) {
        end_ = &sentinel_;
      }
    } else {
      Node* previous = sentinel_.next_;
      for (uint_32_cx i = 0; i < index - 1; ++i) {
        previous = previous->next_;
      }
      toDelete = previous->next_;
      previous->next_ = toDelete->next_;
      if (toDelete == end_) {
        end_ = previous;
      }
    }

    val = toDelete->val_;
    delete toDelete;
    size_--;

    return val;
  }
  /**
 * Removes the last element of this LinkedList<p>
   * <b>This operation is very slow (O(n) runtime).</b>
   * In a Singly Linked List each node doesnt have access to the previous
   * so in order to delete the last it has to iterate over all nodes to update the pointers.
  */
  inline void pop() {
    CX_ASSERT(sentinel_.next_ && "list is empty");

    Node* toDelete = end_;

    if (end_ == sentinel_.next_) {
      sentinel_.next_ = nullptr;
      end_ = &sentinel_;
      size_--;
    } else {
      Node* previous = sentinel_.next_;
      while (previous->next_ != end_) {
        previous = previous->next_;
      }
      previous->next_ = nullptr;
      end_ = previous;
      size_--;
    }

    delete toDelete;
  }
  /**
   * @return a reference to the last element
   */
  [[nodiscard]] inline T& back() {
    CX_ASSERT(end_ != &sentinel_ && "no such element");
    return end_->get();
  }
  /**
   * Removes the first node with the given value
   * @param val - the value to be matched
   */
  inline void erase(const T& val) {
    CX_ASSERT(sentinel_.next_ && "list is empty");

    if (sentinel_.next_->val_ == val) {
      Node* toDelete = sentinel_.next_;
      sentinel_.next_ = toDelete->next_;
      if (!sentinel_.next_) {
        end_ = &sentinel_;
      }
      delete toDelete;
      size_--;
    } else {
      Node* current = sentinel_.next_;
      while (current->next_ && current->next_->val_ != val) {
        current = current->next_;
      }

      if (current->next_) {
        Node* toDelete = current->next_;
        current->next_ = toDelete->next_;
        if (toDelete == end_) {
          end_ = current;
        }
        delete toDelete;
        size_--;
      }
    }
  }
  /**
 * Clears the linked list of all elements
 */
  inline void clear() {
    Node* current = sentinel_.next_;
    while (current != nullptr) {
      Node* next = current->next_;
      delete current;
      current = next;
    }
    sentinel_.next_ = nullptr;
    end_ = &sentinel_;
    size_ = 0;
  }
  /**
 * @return the current size of this Linked List
 */
  [[nodiscard]] inline uint_32_cx size() const { return size_; }
  class Iterator {
   public:
    Node* current;
    explicit Iterator(Node* start) : current(start){};
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
  Iterator begin() { return Iterator(sentinel_.next_); }
  Iterator end() { return Iterator(nullptr); }

  friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& q) {
    Node* current = q.head_;
    while (current != nullptr) {
      os << current->val_ << "->";
      current = current->next_;
    }
    return os;
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_LINKEDLIST_H
