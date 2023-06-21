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
#define CX_ALLOC
#ifndef CXSTRUCTS_TRIE_H
#define CXSTRUCTS_TRIE_H

#pragma message( \
        "|Trie.h| Using non ASCII characters wont throw an error but result in unintentional behaviour!")

#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace cxhelper {
struct TrieNode {
  std::array<TrieNode*, 128> children{};
  bool filled = false;
  std::string word;
  inline void setWord(std::string& s) {
    filled = true;
    word = std::move(s);
  }
};
}  // namespace cxhelper

#ifdef CX_ALLOC
#include "../CXAllocator.h"
using AllocatorType = CXPoolAllocator<cxhelper::TrieNode, 4098, 2>;
#else
using AllocatorType = std::allocator<cxhelper::TrieNode>;
#endif

namespace {
inline constexpr bool using_custom_alloc =
#ifdef CX_ALLOC
    true;
#else
    false;
#endif
}  // namespace

namespace cxstructs {
using namespace cxhelper;

/**
 *Only supports ASCII characters (0-127)
 */
template <class Allocator = AllocatorType>
class Trie {
  TrieNode* root;
  Allocator alloc;
  uint_fast32_t size_;
  static inline uint8_t getASCII(char c) {
    return static_cast<uint8_t>(c) & 0x7F;
  }
  void getAllSubChildren(TrieNode* node, std::vector<std::string>& words) {
    for (auto subNode : node->children) {
      if (subNode) {
        if (subNode->filled) {
          words.emplace_back(subNode->word);
        }
        getAllSubChildren(subNode, words);
      }
    }
  }

 public:
  Trie() : size_(0) {
    TrieNode* temp = alloc.allocate(1);
    std::allocator_traits<Allocator>::construct(alloc, temp);
    root = temp;
  }
  ~Trie() {
    if (!using_custom_alloc) {
      std::vector<TrieNode*> nodesToDelete;
      nodesToDelete.push_back(root);

      while (!nodesToDelete.empty()) {
        TrieNode* node = nodesToDelete.back();
        nodesToDelete.pop_back();

        for (auto child : node->children) {
          if (child != nullptr) {
            nodesToDelete.push_back(child);
          }
        }

        std::allocator_traits<Allocator>::destroy(alloc, node);
        alloc.deallocate(node, 1);
      }
    }
  }
  Trie& operator=(const Trie& o) = delete;
  Trie& operator=(const Trie&& o) = delete;
  Trie(const Trie& o) = delete;
  Trie(const Trie&& o) = delete;

  /**
 *  Inserts the given string into the trie, saving it for lookups
 * @param s the string
 */
  void insert(std::string s) {
    TrieNode* iterator = root;
    uint8_t ascii;
    for (auto& c : s) {
      ascii = getASCII(c);
      if (!iterator->children[ascii]) {
        TrieNode* temp = alloc.allocate(1);
        std::allocator_traits<Allocator>::construct(alloc, temp);
        iterator->children[ascii] = temp;
      }
      iterator = iterator->children[ascii];
    }
    iterator->setWord(s);
    size_++;
  }
  /**
   * This function checks if the provided string is present in the Trie.
   * @param s  A string query
   * @return true if s is inside the trie
   */
  bool contains(const std::string& s) {
    TrieNode* iterator = root;
    for (auto& c : s) {
      iterator = iterator->children[getASCII(c)];
      if (!iterator) {
        return false;
      }
    }
    return iterator->filled;
  }
  /**
 *
 * This function returns all words in the Trie that start with the provided prefix.
 * If no words are found with the given prefix, it returns an empty vector.
 *
 * @param prefix A string that serves as the prefix for the search.
 * @return A vector of strings where each string is a word that begins with the given prefix.
 */
  std::vector<std::string> startsWith(const std::string& prefix) {
    TrieNode* iterator = root;
    for (auto& c : prefix) {
      iterator = iterator->children[getASCII(c)];
      if (!iterator) {
        return {};
      }
    }
    if (iterator) {
      std::vector<std::string> re{};
      getAllSubChildren(iterator, re);
      return re;
    } else {
      return {};
    }
  }
  /**
   *
   * @return the total amount of words in the trie
   */
  [[nodiscard]] uint_fast32_t size() const { return size_; }
  /**
   * Empty check on the trie
   * @return true if the trie contains no words
   */
  [[nodiscard]] bool empty() const { return size_ == 0; }
  /**
   * Clears the trie of all words
   */
  void clear() {
    if (using_custom_alloc) {

    } else {
      std::vector<TrieNode*> nodesToDelete;
      nodesToDelete.push_back(root);

      while (!nodesToDelete.empty()) {
        TrieNode* node = nodesToDelete.back();
        nodesToDelete.pop_back();

        for (auto child : node->children) {
          if (child != nullptr) {
            nodesToDelete.push_back(child);
          }
        }

        std::allocator_traits<Allocator>::destroy(alloc, node);
        alloc.deallocate(node, 1);
      }
      size_ = 0;
      TrieNode* temp = alloc.allocate(1);
      std::allocator_traits<Allocator>::construct(alloc, temp);
      root = temp;
    }
  }
  /**
   * Removes the given word from the trie
   */
  void remove(const std::string& s) { size_--; }
  static void TEST() {
    std::cout << "TESTING TRIE" << std::endl;

    std::cout << "   Testing insertion..." << std::endl;

    Trie trie;
    trie.insert("hello");

    std::cout << "   Testing contains..." << std::endl;
    assert(trie.contains("hello") == true);
    assert(trie.contains("helloh") == false);
    std::cout << "   Testing startsWith..." << std::endl;
    assert(trie.startsWith("he")[0] == "hello");
  }
};
}  // namespace cxstructs
#undef CX_ALLOC
#endif  //CXSTRUCTS_TRIE_H
