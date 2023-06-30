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
#include <deque>
#include <functional>
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
  HashListNode(const K& key, const V& val)
      : key_(key), value_(val), next_(nullptr) {}
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
  HashLinkedList& operator=(const HashLinkedList& o) {
    if (this != &o) {
      std::copy(o.data_, o.data_ + ArrayLength, data_);

      if (o.head_) {
        head_ = new HashListNode<K, V>(o.head_, o.head_->next_);
        HashListNode<K, V>* current_new = head_;
        HashListNode<K, V>* current_old = o.head_->next_;

        while (current_old != nullptr) {
          current_new->next_ = new HashListNode<K, V>(current_old);

          current_new = current_new->next_;
          current_old = current_old->next_;
        }
        end_ = current_new;
      } else {
        head_ = end_ = nullptr;
      }
    }
    return *this;
  }
  ~HashLinkedList() {
    HListNode* current = head_;
    while (current != nullptr) {
      HListNode* next = current->next_;
      delete current;
      current = next;
    }
  }
  inline V& operator[](const K& key) {
    for (uint_16_cx i = 0; i < ArrayLength; i++) {
      if (data_[i].assigned() && data_[i].first() == key) {
        return data_[i].second();
      }
    }

    HListNode* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        return current->value_;
      }
      current = current->next_;
    }
    return data_[0].second();
  }
  inline V& at(const K& key) {
    for (uint_16_cx i = 0; i < ArrayLength; i++) {
      if (data_[i].assigned() && data_[i].first() == key) {
        return data_[i].second();
      }
    }

    HListNode* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        return current->value_;
      }
      current = current->next_;
    }
    throw std::out_of_range("no such key");
  }
  inline bool replaceAdd(const K& key, const V& val) {
    for (int i = 0; i < ArrayLength; i++) {
      if (data_[i].assigned() && data_[i].first() == key) {
        data_[i].second() = val;
        return false;
      } else if (!data_[i].assigned()) {
        data_[i].assigned() = true;
        data_[i].first() = key;
        data_[i].second() = val;
        return true;
      }
    }

    HListNode* current = head_;
    while (current != nullptr) {
      if (current->key_ == key) {
        current->value_ = val;
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
  inline bool remove(const K& key) {
    for (uint_16_cx i = 0; i < ArrayLength; i++) {
      if (data_[i].assigned() && data_[i].first() == key) {
        data_[i].assigned() = false;
        return true;
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
        return true;
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
          return true;
        }
      }
    }
    return false;
    // throw std::out_of_range("no such element");
  }
  inline bool contains(const K& key) {
    for (uint_fast32_t i = 0; i < ArrayLength; i++) {
      if (data_[i].first() == key) {
        return true;
      }
    }
    HashListNode<K, V>* it = head_;
    while (it) {
      if (it->key_ == key) {
        return true;
      }
      it = it->next_;
    }
    return false;
  }
};

template <typename K>
struct KeyHash {
  size_t operator()(const K& key) const {
    return static_cast<size_t>(key);
  }
};

template <>
struct KeyHash<std::string> {
  size_t operator()(const std::string& key) const {
    std::hash<std::string> hash_fn;
    return hash_fn(key);
  }
};

template <>
struct KeyHash<float> {
  size_t operator()(const float& key) const {
    std::hash<float> hash_fn;
    return hash_fn(key);
  }
};

template <>
struct KeyHash<double> {
  size_t operator()(const double& key) const {
    std::hash<double> hash_fn;
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

  constexpr static uint_16_cx BufferLen = 2;
  using hash_func = std::function<int(const K&)>;
  using HList = HashLinkedList<K, V, BufferLen>;

  uint_32_cx initialCapacity_;
  uint_32_cx size_;
  uint_32_cx buckets_;
  uint_32_cx maxSize;
  float load_factor_;

  HList* arr_;
  hash_func hash_func_;

  inline void reHashBig() {
    // once the n_elem limit is reached all values needs to be rehashed to fit to the keys with the new bucket n_elem
    auto oldBuckets = buckets_;
    buckets_ = buckets_ * 2;
    auto* newArr = new HList[buckets_];

    for (int i = 0; i < oldBuckets; i++) {
      for (uint_fast32_t j = 0; j < BufferLen; j++) {
        size_t hash = hash_func_(arr_[i].data_[j].first()) % buckets_;
        newArr[hash].replaceAdd(arr_[i].data_[j].first(),
                                arr_[i].data_[j].second());
      }
      HashListNode<K, V>* current = arr_[i].head_;
      while (current) {
        size_t hash = hash_func_(current->key_) % buckets_;
        newArr[hash].replaceAdd(current->key_, current->value_);
        current = current->next_;
      }
    }
    delete[] arr_;
    arr_ = newArr;
    maxSize = buckets_ * load_factor_;
  }
  inline void reHashSmall() {
    //only used in shrink_to_fit()
    auto oldBuckets = buckets_;
    buckets_ = size_ * 1.5;
    auto* newArr = new HList[buckets_];

    for (int i = 0; i < oldBuckets; i++) {
      for (uint_fast32_t j = 0; j < BufferLen; j++) {
        size_t hash = hash_func_(arr_[i].data_[j].first()) % buckets_;
        newArr[hash].replaceAdd(arr_[i].data_[j].first(),
                                arr_[i].data_[j].second());
      }
      HashListNode<K, V>* current = arr_[i].head_;
      while (current) {
        size_t hash = hash_func_(current->key_) % buckets_;
        newArr[hash].replaceAdd(current->key_, current->value_);
        current = current->next_;
      }
    }
    delete[] arr_;
    arr_ = newArr;
    maxSize = buckets_ * load_factor_;
  }

 public:
  explicit HashMap(uint_32_cx initialCapacity = 64, float loadFactor = 0.75)
      : buckets_(initialCapacity),
        initialCapacity_(initialCapacity),
        size_(0),
        arr_(new HList[initialCapacity]),
        maxSize(initialCapacity * 0.75),
        hash_func_(KeyHash<K>{}),
        load_factor_(loadFactor) {}
  /**
   * This constructor allows the user to supply their own hash function for the key type
   * @tparam HashFunction callable that takes a key with type K and returns int
   * @param hash_function  this function is called with any given key with type K
   * @param initialCapacity the initial size of the container and the growth size
   */
  template <
      typename HashFunction,
      typename = std::enable_if_t<std::is_invocable_r_v<int, HashFunction, K>>>
  explicit HashMap(HashFunction hash_function, uint_32_cx initialCapacity = 64,
                   float loadFactor = 0.75)
      : buckets_(initialCapacity),
        initialCapacity_(initialCapacity),
        size_(0),
        arr_(new HList[initialCapacity]),
        maxSize(initialCapacity * 0.75),
        hash_func_(hash_function),
        load_factor_(loadFactor) {}
  HashMap(const HashMap<K, V>& o)
      : initialCapacity_(o.initialCapacity_),
        size_(o.size_),
        buckets_(o.buckets_),
        hash_func_(o.hash_func_),
        maxSize(o.maxSize),
        load_factor_(o.load_factor_) {
    arr_ = new HList[buckets_];
    for (uint_32_cx i = 0; i < buckets_; i++) {
      arr_[i] = o.arr_[i];
    }
  }
  HashMap(HashMap<K, V>&& o) noexcept
      : initialCapacity_(o.initialCapacity_),
        size_(o.size_),
        buckets_(o.buckets_),
        hash_func_(std::move(o.hash_func_)),
        maxSize(o.maxSize),
        load_factor_(o.load_factor_),
        arr_(o.arr_) {
    o.arr_ = nullptr;
    o.size_ = 0;
  }
  HashMap& operator=(const HashMap<K, V>& o) {
    if (this != &o) {
      delete[] arr_;

      initialCapacity_ = o.initialCapacity_;
      size_ = o.size_;
      buckets_ = o.buckets_;
      hash_func_ = o.hash_func_;
      maxSize = o.maxSize;

      arr_ = new HList[buckets_];
      for (uint_32_cx i = 0; i < buckets_; ++i) {
        arr_[i] = o.arr_[i];
      }
    }
    return *this;
  }
  HashMap& operator=(HashMap<K, V>&& o) noexcept {
    if (this != &o) {
      delete[] arr_;

      initialCapacity_ = o.initialCapacity_;
      size_ = o.size_;
      buckets_ = o.buckets_;
      hash_func_ = std::move(o.hash_func_);
      maxSize = o.maxSize;
      arr_ = o.arr_;

      o.arr_ = nullptr;
      o.size_ = 0;
    }
    return *this;
  }
  ~HashMap() { delete[] arr_; };
  /**
   * Retrieves the value for the given key<p>
   * If the key doesnt exist will return a dummy value
   * @param key - the key to the value
   * @return the value at this key
   */
  inline V& operator[](const K& key) const {
    return arr_[hash_func_(key) % buckets_][key];
  }
  /**
   * Inserts a key, value pair into the map
   * @param key - the key to access the stored element
   * @param val - the stored value at the given key
   */
  inline void insert(const K& key, const V& val) {
    if (size_ > maxSize) {
      reHashBig();
    }
    size_ += arr_[hash_func_(key) % buckets_].replaceAdd(key, val);
  }
  /**
   * Retrieves the value for the given key <p>
   * <b>Throws an std::out_of_range if the key doesnt exist</b>
   * @param key - the key to the value
   * @return the value at this key
   */
  [[nodiscard]] inline V& at(const K key) const {
    return arr_[hash_func_(key) % buckets_].at(key);
  }
  /**
   * Removes this key, value pair from the hashmap
   * @param key - they key to be removed
   */
  inline void erase(const K& key) {
    size_t hash = hash_func_(key) % buckets_;
    size_ -= arr_[hash].remove(key);
    assert(size_ > 0 && "out of bounds");
  }
  /**
   *
   * @return the current n_elem of the hashMap
   */
  [[nodiscard]] inline uint_32_cx size() const { return size_; }
  /**
   * The initialCapacity the hashmaps started with and expands along
   * @return the initial capacity
   */
  [[nodiscard]] inline uint_32_cx capacity() const { return buckets_; }
  /**
   * Clears the hashMap of all its contents
   */
  inline void clear() {
    delete[] arr_;
    arr_ = new HList[initialCapacity_];
    buckets_ = initialCapacity_;
    size_ = 0;
    maxSize = buckets_ * load_factor_;
  }
  /**
 * @brief Checks if the HashMap contains a specific key.
 *
 * @param key The key to search for in the HashMap.
 * @return true if the key is present in the HashMap, false otherwise.
 */
  inline bool contains(const K& key) {
    return arr_[hash_func_(key) % buckets_].contains(key);
  }
  /**
   * Reduces the underlying array size to something close to the actual data size.
   * This decreases memory usage.
   */
  inline void shrink_to_fit() {
    if (buckets_ > size_ * 1.5) {
      reHashSmall();
    }
  }
  class ValueIterator {
    std::deque<V> values_;
    explicit ValueIterator(HList* h_list, uint_32_cx size)
        : values_(){

          };
  };
#ifndef CX_DELETE_TESTS
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
      std::string nodiscard = map1.at(1);
      assert(false);
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
    std::cout << map3[2] << std::endl;
    // Test n_elem
    std::cout << "  Testing size()..." << std::endl;
    assert(map1.size() == 1);
    assert(map2.size() == 1);
    assert(map3.size() == 1);

    // Test at
    std::cout << "  Testing at method..." << std::endl;
    assert(map1.at(2) == "Two");
    std::cout << map3[2] << std::endl;
    // Test clear
    std::cout << "  Testing clear method..." << std::endl;
    std::cout << map3[2] << std::endl;
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
    std::cout << "  Testing contains method..." << std::endl;
    HashMap<int, std::string> map5;
    map5.insert(1, "One");
    map5.insert(2, "Two");
    assert(map5.contains(1));
    assert(map5.contains(2));
    assert(!map5.contains(3));

    // Test shrink_to_fit
    std::cout << "  Testing shrink_to_fit method..." << std::endl;
    map5.shrink_to_fit();
    assert(map5.capacity() == map5.size() * 1.5);
    HashMap<int, int> map6;
    for (int i = 0; i < 10000; i++) {
      map6.insert(i, i * 2);
    }
    for (int i = 0; i < 10000; i++) {
      assert(map6[i] == i * 2);
    }
    map6.shrink_to_fit();
    assert(map6.capacity() == map6.size() * 1.5);

    // Test move constructor
    std::cout << "  Testing move constructor..." << std::endl;
    HashMap<int, std::string> map7(std::move(map3));
    assert(map7[2] == "Two");
    assert(map3.size() == 0);

    // Test move assignment operator
    std::cout << "  Testing move assignment operator..." << std::endl;
    HashMap<int, std::string> map8;
    map8 = std::move(map2);
    assert(map8[2] == "Two");
    assert(map2.size() == 0);

    // Test destructor
    std::cout << "  Testing destructor indirectly..." << std::endl;
    {
      HashMap<int, std::string> map9;
      map9.insert(1, "One");
    }  // map9 goes out of scope here, its destructor should be called.

    // Stress test
    std::cout << "  Stress testing..." << std::endl;
    HashMap<int, int> map10;
    for (int i = 0; i < 1000000; i++) {
      map10.insert(i, i);
    }
    for (int i = 0; i < 1000000; i += 2) {
      map10.erase(i);
    }
    for (int i = 1; i < 1000000; i += 2) {
      assert(map10[i] == i);
    }
    HashMap<int, std::string> map11;
    // Test at with non-existent key
    std::cout << "  Testing at with non-existent key..." << std::endl;
    try {
      std::string nodiscard = map11.at(3);
      assert(false);
    } catch (const std::exception& e) {
      assert(true);
    }
  }
#endif
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_HASHMAP_H
