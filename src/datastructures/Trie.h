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
#pragma message( \
        "|Trie.h| Using non ASCII characters wont throw an error but result in unintentional behaviour!")
#ifndef CXSTRUCTS_TRIE_H
#define CXSTRUCTS_TRIE_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cxhelper {
struct TrieNode {
  std::array<TrieNode*, 128> children{};
  bool filled = false;
  std::string word;
  TrieNode() = default;
  TrieNode(bool value, std::string s) : filled(value), word(std::move(s)){};
  ~TrieNode() {
    for (auto& i : children) {
      delete i;
    }
  }
  TrieNode* operator[](uint8_t i) { return children[i]; }
  void setWord(std::string s) {
    filled = true;
    word = std::move(s);
  }
};

}  // namespace cxhelper

namespace cxstructs {

/**
 *Only supports ASCII characters (0-127)
 */
class Trie {
  TrieNode* root;

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
  Trie() : root(new TrieNode()) {}
  ~Trie() { delete root; }

  /**
 * Inserts the given string into the trie, saving it for lookups
 * @param s the string
 */
  void insert(std::string s) {
    TrieNode* iterator = root;
    uint8_t ascii;
    for (auto& c : s) {
      ascii = getASCII(c);
      if (!iterator->children[ascii]) {
        iterator->children[ascii] = new TrieNode();
      }
      iterator = iterator->children[ascii];
    }
    iterator->setWord(s);
  }
  /**
   * This function checks if the provided string is present in the Trie.
   * @param s  A string query
   * @return true if s is inside the trie
   */
  bool contains(std::string s) {
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
  std::vector<std::string> complete(std::string prefix) {
    TrieNode* iterator = root;
    for (char c : prefix) {
      uint8_t ascii = getASCII(c);
      auto node = iterator->children[ascii];
      if (!node) {
        return {};
      }
      iterator = node;
    }
    if (iterator) {
      std::vector<std::string> re{};
      getAllSubChildren(iterator, re);
      return re;
    }
    return {};
  }
  static void TEST() {
    std::cout << "TESTING TRIE" << std::endl;

    std::cout << "   Testing insertion..." << std::endl;

    Trie trie;
    trie.insert("hello");

    std::cout << "   Testing contains..." << std::endl;
    assert(trie.contains("hello") == true);
    assert(trie.contains("helloh") == false);
    std::cout << "   Testing complete..." << std::endl;
    assert(trie.complete("he")[0] == "hello");
  }
};
}  // namespace cxstructs

#endif  //CXSTRUCTS_TRIE_H
