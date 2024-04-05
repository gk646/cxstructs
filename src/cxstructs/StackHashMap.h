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
#define CX_FINISHED
#ifndef CXSTRUCTS_SRC_CXSTRUCTS_STACKHASHMAP_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_STACKHASHMAP_H_

#include "../cxconfig.h"
#include <bitset>  //For std::bitset<N> and std::hash<K>

/**
 * StackHashMap is a hash map implemented entirely on the stack with an STL-like interface.<br>
 * It is designed for very fast lookup operations with a fixed size, making it ideal as a high-performance,<br>
 * fixed-capacity lookup table. However, it is slower during build-up, and maintaining a load factor<br>
 * below 90% is recommended to ensure no endless loops.<br>
 *
 * Due to its rehashing strategy to resolve collisions—multiplying hash values by a random number<br>
 * until a perfect hash is found—insert operations can potentially enter an endless loop if a perfect<br>
 * hash for all elements cannot be determined.<br>
 *
 * Performance Characteristics:
 * - Build-up is generally slower compared to other hash maps at equal load factors.<br>
 * - Lookup operations are approximately twice as fast as those in std::unordered_map.<br>
 * - Erase operations are significantly faster due to the nature of the implementation.<br>
 *
 * @tparam K Type of the keys.
 * @tparam V Type of the values.
 * @tparam N Size of the hash map. Must be a compile-time constant.
 * @tparam HashFunc Hash function to be used. Defaults to std::hash<K>.
 * @tparam size_type Type for representing sizes. Defaults to uint32_t.
 *
 * Note: StackHashMap currently only supports trivial types for both keys and values.
 */
template <typename K, typename V, size_t N, typename HashFunc = std::hash<K>,
          typename size_type = uint32_t>
class StackHashMap {
  static_assert(std::is_trivial_v<K> && std::is_trivial_v<V>,
                "StackHashMap only supports trivial types for now");
  using const_key_ref = const K&;
  using const_val_ref = const V&;
  using move_key_ref = K&&;
  using move_val_ref = V&&;
  using key_ref = K&;
  using val_ref = V&;

  struct Node {
    K key;
    V val;
  };

  HashFunc hash_func_;
  Node data_[N];
  bool register_ [N] = {0};
  size_type size_ = 0;
  int rand_ = rand();

  [[nodiscard]] inline constexpr size_t impl_hash_func(const K& key) const noexcept {
    if constexpr ((N & (N - 1)) == 0) {  //Power of 2 optimization
      return hash_func_(key) & (N - 1);
    }
    return (hash_func_(key) * rand_) % N;
  }

  inline void add_node(size_t hash, const_key_ref key, auto val) {
    auto& isSet = register_[hash];
    size_ += isSet;
    isSet = true;
    data_[hash].val = val;
    data_[hash].key = key;
  }

  inline void insert_non_empty(const K& key, auto val) {
    Node org_data[N];
    bool org_register [N];
    std::memcpy(org_data, data_, N * sizeof(Node));
    std::memcpy(org_register, register_, N );

    while (true) {
      std::memset(register_, false, N);
      rand_ = rand();

      bool unique_mapping = true;
      for (int i = 0; i < N; i++) {
        if (!org_register[i]) continue;
        const size_t hash = impl_hash_func(org_data[i].key);
        if (register_[hash]) {  //New-found hash is not unique
          unique_mapping = false;
          break;
        }
        data_[hash].key = org_data[i].key;
        data_[hash].val = org_data[i].val;
        register_[hash] = true;
      }

      if (!unique_mapping) continue;

      const auto new_hash = impl_hash_func(key);
      if (register_[new_hash]) continue;

      // Check integrity
      bool integrity_check = true;

      for (int i = 0; i < N; i++) {
        if (!org_register[i]) continue;
        bool found_old_key = false;
        const auto& old_key = org_data[i].key;
        for (int j = 0; j < N; j++) {
          if (data_[j].key == old_key) {
            found_old_key = true;
            break;
          }
        }
        if (!found_old_key) {
          integrity_check = false;
          break;
        }
      }

      if (!integrity_check) continue;

      add_node(new_hash, key, val);
      break;
    }
  }

 public:
  StackHashMap() = default;
  explicit StackHashMap(size_type elems) : size_(elems) {}

  //Call can result in endless loop at high load factor
  inline void insert(const K& key, V&& val) {
    CX_ASSERT(size_ < N, "Trying to add to full StackHashMap");
    CX_STACK_ABORT_IMPL();

    const size_t hash = impl_hash_func(key);

    if (register_[hash] && data_[hash].key != key) {
      return insert_non_empty(key, val);
    }

    add_node(hash, key, val);
  }

  //Call can result in endless loop at high load factor
  inline void insert(const K& key, const V& val) {
    CX_ASSERT(size_ < N, "Trying to add to full StackHashMap");
    CX_STACK_ABORT_IMPL();

    const size_t hash = impl_hash_func(key);

    if (register_[hash] && data_[hash].key != key) {
      return insert_non_empty(key, val);
    }

    add_node(hash, key, val);
  }

  inline void insert(const std::pair<const K, V>& keyValue) {
    CX_ASSERT(size_ < N, "Trying to add to full StackHashMap");
    CX_STACK_ABORT_IMPL();

    const K& key = keyValue.first;
    const V& val = keyValue.second;

    const size_t hash = impl_hash_func(key);

    if (register_[hash] && data_[hash].key != key) {
      return insert_non_empty(key, val);
    }

    add_node(hash, key, val);
  }

  inline V& operator[](const K& key) noexcept {
    const size_t hash = impl_hash_func(key);

    if (!register_[hash]) {
      add_node(hash, key, V());
      return data_[hash].val;
    } else if (data_[hash].key == key) {
      return data_[hash].val;
    }

    insert(key, V());
    return data_[impl_hash_func(key)].val;
  }

  [[nodiscard]] size_type size() const noexcept { return size_; }

  inline bool erase(const K& key) noexcept {
    const size_t hash = impl_hash_func(key);
    if (register_[hash]) {
      register_[hash] = false;
      return true;
    }
    return false;
  }

  [[nodiscard]] inline bool contains(const K& key) const noexcept {
    const size_t hash = impl_hash_func(key);
    return register_[hash] && data_[hash].key == key;
  }

  inline void clear() noexcept {
    register_.reset();
    size_ = 0;
  }

  [[nodiscard]] inline bool empty() const noexcept { return register_.any(); }

  [[nodiscard]] inline float load_factor() const noexcept { return (float)size_ / (float)N; }

#ifdef CX_INCLUDE_TESTS
  static void TEST() {
    StackHashMap<int, int, 100> map1;

    map1.insert(1, 10);
    map1.insert(2, 20);
    map1.insert(3, 30);

    CX_ASSERT(map1.size() == 3, "Size has to be 3");

    CX_ASSERT(map1[3] == 30, "Key 3 is 30");

    map1.insert(101, 1010);
    CX_ASSERT(map1.size() == 4, "Size has to be 4");
    CX_ASSERT(map1[101] == 1010, "Key 101 has to be 1010 ");

    bool res = map1.erase(101);
    CX_ASSERT(res, "Value was deleted");
    CX_ASSERT(!map1.contains(101), "Key is not contained");
    CX_ASSERT(map1[101] == 0, "Key was deleted and remade");

    map1.clear();

    for (int i = 0; i < 50; i++) {
      printf("%f\n", map1.load_factor());
      map1.insert(i, i * 10);
      CX_ASSERT(map1[i] == i * 10, "");
    }

    for (int i = 0; i < 50; i++) {
      CX_ASSERT(map1[i] == i * 10, "");
    }
  }
#endif
};
#endif  //CXSTRUCTS_SRC_CXSTRUCTS_STACKHASHMAP_H_
