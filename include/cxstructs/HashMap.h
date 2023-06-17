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
#ifndef CXSTRUCTS_HASHMAP_H
#define CXSTRUCTS_HASHMAP_H

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>

// HashMap implementation using a linked lists as bucket structure
// Optimized towards large additions and deletions
// Can perform up to 20% faster than the std::unordered_map on large additions
// or deletions but can be up to 2 times slower on small numbers
// See Comparison.h

namespace cxhelper {  // namespace to hide the classes
/**
 * HashListNode used in the HashLinkedList
 * @tparam K - key type
 * @tparam V - value type
 */
template <typename K, typename V>
struct HashListNode {
  inline explicit HashListNode(K key, V value)
      : key_(key), value_(value), next_(nullptr){};
  inline HashListNode(const HashListNode<K, V>& o)
      : key_(o.key_),
        value_(o.value_),
        next_(o.next_ ? new HashListNode<K, V>(*o.next_) : nullptr) {}
  K key_;
  V value_;
  HashListNode* next_;
};
/**
 * HashLinkedList used in the buckets of the HashMap
 *@tparam K - key type
 * @tparam V - value type
 */
template <typename K, typename V>
struct HashLinkedList {
  HashListNode<K, V>* head_;
  HashListNode<K, V>* end_;
  uint_fast32_t size_;
  HashLinkedList() : head_(nullptr), end_(nullptr), size_(0){};
  ~HashLinkedList() { clear(); }
  HashLinkedList(const HashLinkedList<K, V>& o) : size_(o.size_) {
    if (o.head_) {
      head_ = new HashListNode<K, V>(*o.head_);

      HashListNode<K, V>* current_new = head_;
      HashListNode<K, V>* current_old = o.head_->next_;

      while (current_old != nullptr) {
        current_new->next_ = new HashListNode<K, V>(*current_old);

        current_new = current_new->next_;
        current_old = current_old->next_;
      }
      end_ = current_new;
    } else {
      head_ = end_ = nullptr;
    }
  }
  HashLinkedList& operator=(const HashLinkedList<K, V>& o) {
    if (this == &o)
      return *this;

    clear();
    size_ = o.size_;

    if (o.head_) {
      head_ = new HashListNode<K, V>(*o.head_);

      HashListNode<K, V>* current_new = head_;
      HashListNode<K, V>* current_old = o.head_->next_;

      while (current_old != nullptr) {
        current_new->next_ = new HashListNode<K, V>(*current_old);

        current_new = current_new->next_;
        current_old = current_old->next_;
      }
      end_ = current_new;
    } else {
      head_ = end_ = nullptr;
    }
    return *this;
  }
  V& operator[](K key) {
    HashListNode<K, V>* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        return current->value_;
      }
      current = current->next_;
    }
    throw std::out_of_range("no such element");
  }
  bool replaceAdd(K key, V val) {
    HashListNode<K, V>* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        current->value_ = val;
        return false;
      }
      current = current->next_;
    }
    add(key, val);
    return true;
  }
  inline void remove(K key) {
    if (!head_)
      throw std::out_of_range("list is empty");

    if (head_->key_ == key) {
      HashListNode<K, V>* toDelete = head_;
      head_ = head_->next_;
      if (!head_) {
        end_ = nullptr;
      }
      delete toDelete;
    } else {
      HashListNode<K, V>* current = head_;
      while (current->next_ && current->next_->key_ != key) {
        current = current->next_;
      }

      if (current->next_) {
        HashListNode<K, V>* toDelete = current->next_;
        current->next_ = toDelete->next_;
        if (toDelete == end_) {
          end_ = current;
        }
        delete toDelete;
      }
    }
  }
  inline void add(K key, V val) {
    if (head_ == nullptr) {
      head_ = new HashListNode<K, V>(key, val);
      end_ = head_;
    } else {
      end_->next_ = new HashListNode<K, V>(key, val);
      end_ = end_->next_;
    }
    size_++;
  }
  void clear() {
    HashListNode<K, V>* current = head_;
    while (current != nullptr) {
      HashListNode<K, V>* next = current->next_;
      delete current;
      current = next;
    }
    head_ = nullptr;
    end_ = nullptr;
  }
  bool isEmpty() { return size_ == 0; }
};

template <typename K>
struct KeyHash {
  size_t operator()(const K& key) const { return static_cast<size_t>(key); }
};

template <>
struct KeyHash<std::string> {
  size_t operator()(const std::string& key) const {
    std::hash<std::string> hash_fn;
    return hash_fn(key);
  }
};
}  // namespace cxhelper
namespace cxstructs {
using namespace cxhelper;
/**
 * Linear probing HashMap. Apart from primitives and strings you should provide
 * your own hashFunction.
 *
 */
template <typename K, typename V>
class HashMap {
  uint_fast32_t initialCapacity_;
  uint_fast32_t size_;
  uint_fast32_t buckets_;
  HashLinkedList<K, V>* arr_;
  KeyHash<K> hash_;
  uint_fast32_t maxSize;
  uint_fast32_t minSize;

  inline void reHashHigh() {
    auto oldBuckets = buckets_;
    buckets_ = buckets_ * 4;
    auto newArr = new HashLinkedList<K, V>[buckets_];

    for (int i = 0; i < oldBuckets; i++) {
      HashListNode<K, V>* current = arr_[i].head_;
      while (current) {
        size_t hash = hash_(current->key_) % buckets_;
        newArr[hash].replaceAdd(current->key_, current->value_);
        current = current->next_;
      }
    }
    delete[] arr_;
    arr_ = newArr;
    maxSize = buckets_ * 0.75;
    minSize = buckets_ * 0.1 <= initialCapacity_ ? 0 : buckets_ * 0.1;
  }

  inline void reHashLow() {
    auto oldBuckets = buckets_;
    buckets_ = buckets_ / 2;

    auto newArr = new HashLinkedList<K, V>[buckets_];
    for (int i = 0; i < oldBuckets; i++) {
      HashListNode<K, V>* current = arr_[i].head_;
      while (current) {
        size_t hash = hash_(current->key_) % buckets_;
        newArr[hash].replaceAdd(current->key_, current->value_);
        current = current->next_;
      }
    }
    delete[] arr_;
    arr_ = newArr;
    maxSize = buckets_ * 0.75;
    minSize = buckets_ * 0.1 <= initialCapacity_ ? 0 : buckets_ * 0.1;
  }

 public:
  explicit HashMap(uint_fast32_t initialCapacity = 64)
      : buckets_(initialCapacity),
        initialCapacity_(initialCapacity),
        size_(0),
        arr_(new HashLinkedList<K, V>[buckets_]) {
    maxSize = buckets_ * 0.75;
    minSize = 0;
  }
  ~HashMap() { delete[] arr_; };
  HashMap(const HashMap<K, V>& o)
      : initialCapacity_(o.initialCapacity_),
        size_(o.size_),
        buckets_(o.buckets_),
        hash_(o.hash_),
        maxSize(o.maxSize),
        minSize(o.minSize) {
    arr_ = new HashLinkedList<K, V>[buckets_];
    for (uint_fast32_t i = 0; i < buckets_; ++i) {
      arr_[i] = o.arr_[i];
    }
  }
  HashMap& operator=(const HashMap<K, V>& o) {
    if (this == &o)
      return *this;

    delete[] arr_;

    initialCapacity_ = o.initialCapacity_;
    size_ = o.size_;
    buckets_ = o.buckets_;
    hash_ = o.hash_;
    maxSize = o.maxSize;
    minSize = o.minSize;

    arr_ = new HashLinkedList<K, V>[buckets_];
    for (uint_fast32_t i = 0; i < buckets_; ++i) {
      arr_[i] = o.arr_[i];
    }

    return *this;
  }
  /**
   * Retrieves the value for the given key
   * @param key - the key to the value
   * @return the value at this key
   */
  V& operator[](const K key) const {
    size_t hash = hash_(key) % buckets_;
    return arr_[hash][key];
  }
  /**
   * Inserts a key, value pair into the map
   * @param key - the key to access the stored element
   * @param val - the stored value at the given key
   */
  inline void insert(K key, V val) {
    size_t hash = hash_(key) % buckets_;
    size_ += arr_[hash].replaceAdd(key, val);
    if (size_ > maxSize) {
      reHashHigh();
    }
  }
  /**
   * Retrieves the value for the given key
   * @param key - the key to the value
   * @return the value at this key
   */
  [[nodiscard]] V& get(const K key) const {
    size_t hash = hash_(key) % buckets_;
    return arr_[hash][key];
  }
  /**
   * Removes this key, value pair from the hashmap
   * @param key - they key to be removed
   */
  void remove(K key) {
    size_t hash = hash_(key);
    arr_[hash % buckets_].remove(key);
    size_--;
    if (size_ < minSize) {
      reHashLow();
    }
  }
  /**
   *
   * @return the current size of the hashMap
   */
  [[nodiscard]] uint_fast32_t size() const { return size_; }

  /**
   * Clears the hashMap of all its contents
   */
  void clear() {
    delete[] arr_;
    arr_ = new HashLinkedList<K, V>[initialCapacity_];
    buckets_ = initialCapacity_;
    size_ = 0;
    maxSize = buckets_ * 0.75;
    minSize = 0;
  }

  static void TEST() {
    std::cout << "HASHMAP TESTS" << std::endl;
    // Test insert and operator[key]
    std::cout << "  Testing insertion and operator[key]..." << std::endl;
    HashMap<int, std::string> map1;
    map1.insert(1, "One");
    map1.insert(2, "Two");
    assert(map1[1] == "One");
    assert(map1[2] == "Two");

    // Test replace value
    std::cout << "  Testing replacement of values..." << std::endl;
    map1.insert(1, "One_Updated");
    assert(map1[1] == "One_Updated");

    // Test remove
    std::cout << "  Testing remove method..." << std::endl;
    map1.remove(1);
    try {
      std::string nodiscard = map1.get(1);
      assert(
          false);  // We should not reach here, as an exception should be thrown
    } catch (const std::exception& e) {
      assert(true);
    }

    // Test copy constructor
    std::cout << "  Testing copy constructor..." << std::endl;
    HashMap<int, std::string> map2(map1);
    assert(map2[2] == "Two");

    // Test assignment operator
    std::cout << "  Testing assignment operator..." << std::endl;
    HashMap<int, std::string> map3;
    map3 = map1;
    assert(map3[2] == "Two");

    // Test size
    std::cout << "  Testing size method..." << std::endl;
    assert(map1.size() == 1);
    assert(map2.size() == 1);
    assert(map3.size() == 1);

    // Test get
    std::cout << "  Testing get method..." << std::endl;
    assert(map1.get(2) == "Two");

    // Test clear
    std::cout << "  Testing clear method..." << std::endl;
    map1.clear();
    assert(map1.size() == 0);

    // Test large additions
    std::cout << "  Testing large additions..." << std::endl;
    HashMap<int, double> map4;
    for (int i = 0; i < 100000; i++) {
      map4.insert(i, i * 2);
    }
    for (int i = 0; i < 100000; i++) {
      assert(map4[i] == i * 2);
    }
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_HASHMAP_H
