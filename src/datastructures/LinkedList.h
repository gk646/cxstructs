//
// Created by gk646 on 10.06.2023.
//

#ifndef DSA_LINKEDLIST_H
#define DSA_LINKEDLIST_H

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <iostream>

template <typename T> struct ListNode {
  explicit ListNode(T val) : val_(val), next_(nullptr){};
  T val_;
  ListNode *next_;
};

template <typename T> class LinkedList {
  ListNode<T> *head_;
  uint_fast32_t size_;
  ListNode<T> *end_;

public:
  LinkedList() : head_(nullptr), end_(nullptr), size_(0){};
  ~LinkedList() { clear(); }

  /**
   * Adds a new element to the end of the list
   * @param val - the element to be added
   */
  void add(T val) {
    if (!head_) {
      head_ = new ListNode<T>(val);
      end_ = head_;
    } else {
      end_->next_ = new ListNode<T>(val);
      end_ = end_->next_;
    }
    size_++;
  }
  /**
   * Removes the element at index counting from the start node
   * @param index  - the index at which to remove the element
   * @return the element removed with this operation
   */
  T removeAt(uint_fast32_t index) {
    if (index >= size_)
      throw std::out_of_range("index too big");

    ListNode<T> *toDelete;
    T val;
    if (index == 0) {
      toDelete = head_;
      head_ = head_->next_;
      if (size_ == 1) {
        end_ = nullptr;
      }
    } else {
      ListNode<T> *previous = head_;
      for (uint_fast32_t i = 0; i < index - 1; ++i) {
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
    --size_;

    return val;
  }

  /**
   * Removes the last element of this LinkedList
   *
   */
  void remove() {
    if (!end_)
      throw std::out_of_range("list is empty");

    ListNode<T> *toDelete = end_;

    if (end_ == head_) {
      head_ = nullptr;
      end_ = nullptr;
    } else {
      ListNode<T> *previous = head_;
      while (previous->next_ != end_) {
        previous = previous->next_;
      }
      previous->next_ = nullptr;
      end_ = previous;
    }
    --size_;
    delete toDelete;
  }
  /**
   * Removes the first node with the given value
   * @param val - the value to be matched
   */
  void remove(T val) {
    if (!head_)
      throw std::out_of_range("list is empty");

    if (head_->val_ == val) {
      ListNode<T> *toDelete = head_;
      head_ = head_->next_;
      if (!head_) {
        end_ = nullptr;
      }
      delete toDelete;
      --size_;
    } else {
      ListNode<T> *current = head_;
      while (current->next_ && current->next_->val_ != val) {
        current = current->next_;
      }

      if (current->next_) {
        ListNode<T> *toDelete = current->next_;
        current->next_ = toDelete->next_;
        if (toDelete == end_) {
          end_ = current;
        }
        delete toDelete;
        --size_;
      }
    }
  }

  friend std::ostream & operator<<( std::ostream &os, const LinkedList<T> &q){
    ListNode<T> *current = q.head_;
    while (current!= nullptr) {
      os <<current->val_<<"->";
      current = current->next_;
    }
    return os;
  }
  /**
   * Clears the linked list of all elements
   */
  void clear() {
    ListNode<T> *current = head_;
    while (current != nullptr) {
      ListNode<T> *next = current->next_;
      delete current;
      current = next;
    }
    head_ = nullptr;
    end_ = nullptr;
    size_ = 0;
  }
  ListNode<T> &last() { return end_; }
  /**
   *
   * @return the current size of this Linked List
   */
  [[nodiscard]] uint_fast32_t size() const { return size_; }

  class Iterator {
  public:
    ListNode<T> *current;

    explicit Iterator(ListNode<T> *start) : current(start){};

    T operator*() { return current->val_; }

    Iterator &operator++() {
      if (current) {
        current = current->next_;
      }
      return *this;
    }
    bool operator==(const Iterator &other) const {
      return current == other.current;
    }
    bool operator!=(const Iterator &other) const {
      return current != other.current;
    }
  };

  Iterator begin() { return Iterator(head_); }

  Iterator end() { return Iterator(nullptr); }

  static void TEST() {

    LinkedList<int> list;

    list.add(1);
    assert(list.size() == 1);

    list.add(2);
    assert(list.size() == 2);

    // Testing iterator functionality along with add
    auto it = list.begin();
    assert(*it == 1);
    ++it;
    assert(*it == 2);
    ++it;
    assert(it == list.end());

    std::cout << "Add test passed\n";

    LinkedList<int> list2;

    list2.add(1);
    list2.add(2);
    list2.add(3);

    auto removedNode = list2.removeAt(1);
    assert(removedNode == 2);

    assert(list2.size() == 2);

    removedNode = list2.removeAt(0);
    assert(removedNode == 1);

    assert(list2.size() == 1);

    removedNode = list2.removeAt(0);
    assert(removedNode == 3);

    assert(list2.size() == 0);

    std::cout << "Remove test passed\n";

    LinkedList<int> list3;

    list3.add(1);
    list3.add(2);
    list3.add(3);
    list3.clear();

    assert(list3.size() == 0);
    assert(list3.begin() == list3.end());

    std::cout << "Clear test passed\n";

    const int LARGE_NUMBER = 1000000; // 1 million

    for (int k = 0; k < 100; k++) {
      LinkedList<int> list4;
      for (int i = 0; i < LARGE_NUMBER; i++) {
        list4.add(i);
      }

      assert(list4.size() == LARGE_NUMBER);

      for (int i = 0; i < LARGE_NUMBER; i++) {
        list4.removeAt(0);
      }
      assert(list4.size() == 0);
    }

    std::cout << "Memory leak test passed\n";
  }
};
#endif // DSA_LINKEDLIST_H
