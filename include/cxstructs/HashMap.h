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
#include "../cxconfig.h"
#include "pair.h"

// HashMap implementation using constant stack arrays as buffer and linked lists
//already medium well optimized, should perform faster than the std:unordered_map in a lot of scenarios
// See Comparison.h

namespace cxhelper {  // namespace to hide the classes
/**
 * HashListNode used in the HashLinkedList
 * @tparam K - key type
 * @tparam V - value type
 */
template <typename K, typename V>
struct HashListNode {
  HashListNode(K& key, V& val)
      : key_(std::move(key)), value_(std::move(val)), next_(nullptr) {}
  inline explicit HashListNode(HashListNode<K, V>* o)
      : key_(o->key_), value_(o->value_), next_(nullptr){};
  inline HashListNode(HashListNode<K, V>* o, HashListNode<K, V>* next)
      : key_(o->key_), value_(o->value_), next_(next) {}
  K key_;
  V value_;
  HashListNode* next_;
};
/**
 * HashLinkedList used in the buckets of the HashMap
 *@tparam K - key type
 * @tparam V - value type
 */
template <typename K, typename V, uint_16_cx ArrayLength>
struct HashLinkedList {
  using HListNode = HashListNode<K, V>;

  pair<K, V> data_[ArrayLength]{};

  HListNode* head_;
  HListNode* end_;

  HashLinkedList() : head_(nullptr), end_(nullptr){};
  ~HashLinkedList() {
    HListNode* current = head_;
    while (current != nullptr) {
      HListNode* next = current->next_;
      delete current;
      current = next;
    }
  }
  V& operator[](const K& key) {
    for (uint_16_cx i = 0; i < ArrayLength; i++) {
      if (data_[i].assigned && data_[i].first_ == key) {
        return data_[i].second_;
      }
    }
    HListNode* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        return current->value_;
      }
      current = current->next_;
    }
  }
  inline bool replaceAdd(K& key, V& val) {
    for (int i = 0; i < ArrayLength; i++) {
      if (data_[i].assigned && data_[i].first_ == key) {
        data_[i].second_ = std::move(val);
        return false;
      } else if (!data_[i].assigned) {
        data_[i].assigned = true;
        data_[i].first_ = key;
        data_[i].second_ = val;
        return true;
      }
    }

    HListNode* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        current->value_ = std::move(val);
        return false;
      }
      current = current->next_;
    }

    if (head_ == nullptr) {
      head_ = new HashListNode<K, V>(key, val);
      end_ = head_;
    } else {
      end_->next_ = new HashListNode<K, V>(key, val);
      end_ = end_->next_;
    }
    return true;
  }
  inline void remove(const K& key) {
    for (uint_16_cx i = 0; i < ArrayLength; i++) {
      if (data_[i].first_ == key) {
        data_[i].assigned = false;
        return;
      }
    }

    if (head_) {
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
  }
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
 * <h2>Linear Probing HashMap</h2>
 * This data structure is an efficient key-value store, typically providing lookups in constant time (O(1)).
 * <br><br>
 * <b>Important:</b> For non-primitive and non-string types, a custom hash function is required.
 * <br><br>
 * A HashMap is notably beneficial due to its speed and ease of use. Hashing keys to numerical indices allows for quick value retrieval from the underlying array.
 * <br><br>
 * The term 'linear probing' refers to the technique used to handle hash collisions (when different keys produce the same hash). In this scenario, each array index hosts a linked list that is traversed to locate the correct key.
 */

template <typename K, typename V>
class HashMap {
  using HList = HashLinkedList<K, V,2>;
  uint_32_cx initialCapacity_;
  uint_32_cx size_;
  uint_32_cx buckets_;
  HList* arr_;
  KeyHash<K> hash_;
  uint_32_cx maxSize;
  uint_32_cx minSize;

  // once the n_elem limit is reached all values needs to be rehashed to fit to the keys with the new bucket n_elem
  void reHashHigh() {
    auto oldBuckets = buckets_;
    buckets_ = buckets_ * 2;
    auto* newArr = new HList[buckets_];

    for (int i = 0; i < oldBuckets; i++) {
      for (uint_fast32_t j = 0; j < 2; j++) {
        size_t hash = hash_(arr_[i].data_[j].first_) % buckets_;
        newArr[hash].replaceAdd(arr_[i].data_[j].first_,
                                arr_[i].data_[j].second_);
      }
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
    minSize = buckets_ * 0.1 < initialCapacity_ ? 0 : buckets_ * 0.1;
  }

  //only used in shrink_to_fit()
  void reHashLow() {
    auto oldBuckets = buckets_;
    buckets_ = buckets_ / 2;

    auto newArr = new HList[buckets_];
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
    minSize = buckets_ * 0.1 < initialCapacity_ ? 0 : buckets_ * 0.1;
  }

 public:
  explicit HashMap(uint_32_cx initialCapacity = 64)
      : buckets_(initialCapacity),
        initialCapacity_(initialCapacity),
        size_(0),
        arr_(new HList[initialCapacity]) {
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
    arr_ = new HList[buckets_];
    for (uint_32_cx i = 0; i < buckets_; i++) {
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

    arr_ = new HList[buckets_];
    for (uint_32_cx i = 0; i < buckets_; ++i) {
      arr_[i] = o.arr_[i];
    }

    return *this;
  }
  /**
   * Retrieves the value for the given key
   * @param key - the key to the value
   * @return the value at this key
   */
  V& operator[](const K& key) const {
    size_t hash = hash_(key) % buckets_;
    return arr_[hash][key];
  }
  /**
   * Inserts a key, value pair into the map
   * @param key - the key to access the stored element
   * @param val - the stored value at the given key
   */
  inline void insert(K key, V val) {
    if (size_ > maxSize) {
      reHashHigh();
    }
    size_t hash = hash_(key) % buckets_;
    size_ += arr_[hash].replaceAdd(key, val);
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
  void erase(const K& key) {
    size_t hash = hash_(key) % buckets_;
    arr_[hash].remove(key);
    size_--;
  }
  /**
   *
   * @return the current n_elem of the hashMap
   */
  [[nodiscard]] uint_32_cx size() const { return size_; }
  /**
   * The load factor is a %-value of the maximum n_elem the hashmap is allowed tor each before
   * growing and rehashing
   * @return the current load factor of the hashmap
   */
  float loadFactor() { return 0.75; }
  /**
   * The initialCapacity the hashmaps started with and expands along
   * @return the initial capacity
   */
  uint_32_cx initialCapacity() { return initialCapacity_; }
  /**
   * Clears the hashMap of all its contents
   */
  void clear() {
    delete[] arr_;
    arr_ = new HList[initialCapacity_];
    buckets_ = initialCapacity_;
    size_ = 0;
    maxSize = buckets_ * 0.75;
    minSize = 0;
  }
  /**
   *
   * @param key
   * @return
   */
  bool contains(K key) {
    size_t hash = hash_(key) % buckets_;
    if (arr_[hash].size_ == 0) {
      return false;
    } else {
      HashListNode<K, V>* it = arr_[hash_].head_;
      while (it) {
        if (it->key_ == key) {
          return true;
        }
        it = it->next_;
      }
      return false;
    }
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

    // Test erase
    std::cout << "  Testing erase method..." << std::endl;
    map1.erase(1);
    try {
      //std::string nodiscard = map1.get(1);
      //assert(          false);  // We should not reach here, as an exception should be thrown
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

    // Test n_elem
    std::cout << "  Testing n_elem method..." << std::endl;
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
    for (int i = 0; i < 10000; i++) {
      map4.insert(i, i * 2);
    }
    for (int i = 0; i < 10000; i++) {
      assert(map4[i] == i * 2);
    }
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_HASHMAP_H
