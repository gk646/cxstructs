//
// Created by gk646 on 09.06.2023.
//

#ifndef CMAKE_TEST_HASHMAP_H
#define CMAKE_TEST_HASHMAP_H

#include <cstdint>
#include <string>

template <typename K> struct KeyHash {
  size_t operator()(const K &key) const { return static_cast<size_t>(key); }
};

template <> struct KeyHash<std::string> {
  size_t operator()(const std::string &key) const {
    std::hash<std::string> hash_fn;
    return hash_fn(key);
  }
};

template <typename K, typename V> class HashMap {
  uint_fast16_t size_;
  uint_fast16_t buckets_;
  V *arr_;
  KeyHash<K> hash_;

public:
  HashMap() : buckets_(10) , size_(0){ arr_ = new V[10]; }
  HashMap(uint_fast8_t initSize) : buckets_(initSize), size_(0) { arr_ = new V[initSize]; }
  ~HashMap() { delete[] arr_; };

  V &operator[](const K e) {
    size_t hash = hash_(e);
    return arr_[hash % buckets_];
  }

  void put(K key, V val) {
    size_t hash = hash_(key);
    arr_[hash % buckets_] = val;
    size_++;
  }

  V &get(const K e) {
    size_t hash = hash_(e);
    return arr_[hash % buckets_];
  }

  size_t size() const { return size_; }
};

#endif // CMAKE_TEST_HASHMAP_H
