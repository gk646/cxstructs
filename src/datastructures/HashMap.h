//
// Created by gk646 on 09.06.2023.
//

#ifndef CMAKE_TEST_HASHMAP_H
#define CMAKE_TEST_HASHMAP_H

#include <cstdint>
#include <stdexcept>
#include <string>

template <typename K, typename V> struct HashListNode {
  explicit HashListNode(K key, V value)
      : key_(key), value_(value), next_(nullptr){};
  K key_;
  V value_;
  HashListNode *next_;
};

template <typename K, typename V> class HashLinkedList {
  HashListNode<K, V> *head_;
  HashListNode<K, V> *end_;

public:
  HashLinkedList() : head_(nullptr), end_(nullptr){};
  ~HashLinkedList() { clear(); }

  V &operator[](K key) {
    HashListNode<K, V> *next = head_;
    while (next) {
      if (next->key_ == key) {
        return next->value_;
      }
      next = next->next_;
    }
  }
  void remove(K key) {
    if (!head_)
      throw std::out_of_range("list is empty");

    if (head_->key_ == key) {
      HashListNode<K, V> *toDelete = head_;
      head_ = head_->next_;
      if (!head_) {
        end_ = nullptr;
      }
      delete toDelete;
    } else {
      HashListNode<K, V> *current = head_;
      while (current->next_ && current->next_->key_ != key) {
        current = current->next_;
      }

      if (current->next_) {
        HashListNode<K, V> *toDelete = current->next_;
        current->next_ = toDelete->next_;
        if (toDelete == end_) {
          end_ = current;
        }
        delete toDelete;
      }
    }
  }
  void add(K key, V val) {
    if (!head_) {
      head_ = new HashListNode<K, V>(key, val);
      end_ = head_;
    } else {
      end_->next_ = new HashListNode<K, V>(key, val);
      end_ = end_->next_;
    }
  }
  void clear() {
    HashListNode<K, V> *current = head_;
    while (current != nullptr) {
      HashListNode<K, V> *next = current->next_;
      delete current;
      current = next;
    }
    head_ = nullptr;
    end_ = nullptr;
  }
};

template <typename K> struct KeyHash {
  size_t operator()(const K &key) const { return static_cast<size_t>(key); }
};

template <> struct KeyHash<std::string> {
  size_t operator()(const std::string &key) const {
    std::hash<std::string> hash_fn;
    return hash_fn(key);
  }
};
/**
 * Linear probing HashMap. Apart from primitives and strings you should provide
 * your own hashFunction.
 *
 */
template <typename K, typename V> class HashMap {
  uint_fast16_t size_;
  uint_fast16_t buckets_;
  HashLinkedList<K, V> *arr_;
  KeyHash<K> hash_;

public:
  HashMap() : buckets_(16), size_(0) {
    arr_ = new HashLinkedList<K, V>[buckets_];
  }
  HashMap(uint_fast8_t initSize) : buckets_(initSize), size_(0) {
    arr_ = new V[initSize];
  }
  ~HashMap() { delete[] arr_; };

  V &operator[](const K e) {
    size_t hash = hash_(e);
    return arr_[hash % buckets_][e];
  }
  /**
   * Puts a key, value pair into the map
   * @param key - the key to access the stored element
   * @param val - the stored value at the given key
   */
  void insert(K key, V val) {
    size_t hash = hash_(key);
    //add empty check
    arr_[hash % buckets_].add(key, val);
    size_++;
  }

  V &get(const K e) {
    size_t hash = hash_(e);
    return arr_[hash % buckets_][e];
  }
  void remove(K key){
    size_t hash = hash_(key);
    arr_[hash % buckets_].remove(key);
    size_--;
  }
  /**
   *
   * @return the current size of the hashMap
   */
  [[nodiscard]] size_t size() const { return size_; }

  static void TEST(){
    // Testing Insertion and Retrieval
    HashMap<int, std::string> map1;
    map1.insert(1, "One");
    map1.insert(2, "Two");
    assert(map1.get(1) == "One");
    assert(map1.get(2) == "Two");

    // Testing Replacement
    map1.insert(1, "One_Updated");
    std::cout<< map1[1] << std::endl;
    assert(map1.get(1) == "One_Updated");

    // Testing Deletion
    map1.remove(1);
    try {
      map1.get(1);
      assert(false); // We should not reach here, as an exception should be thrown
    } catch (std::exception& e) {
      assert(true);
    }

    // Testing Edge Cases
    // Rehashing or expanding the table when load factor exceeds a certain threshold
    HashMap<int, int> map2;
    for(int i = 0; i < 10000; i++) {
      map2.insert(i, i*2);
    }
    for(int i = 0; i < 10000; i++) {
      assert(map2.get(i) == i*2);
    }

    std::cout << "All tests passed!" << std::endl;
  }
};

#endif // CMAKE_TEST_HASHMAP_H
