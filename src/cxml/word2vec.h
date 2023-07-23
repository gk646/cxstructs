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
#define F
#ifndef CXSTRUCTS_SRC_CXML_WORD2VEC_H_
#define CXSTRUCTS_SRC_CXML_WORD2VEC_H_

#include "../cxconfig.h"
#include "../cxstructs/vec.h"
#include "FNN.h"

namespace cxhelper {}

namespace cxstructs {
class word2vec {

  FNN net;
  uint8_t vec_len;
  uint_32_cx vocab_len;

 public:
  word2vec(int vocabulary_size, int numbers_per_word)
      : net(
            {vocabulary_size, numbers_per_word, vocabulary_size}, [](float x) { return x; }, 0.11,
            cross_entropy),
        vec_len(numbers_per_word),
        vocab_len(vocabulary_size) {}

  void train(const std::vector<string>& sentence, int epochs) {

    mat in(sentence.size(), sentence.size());
    mat target(sentence.size(), sentence.size());

    for (uint_fast32_t i = 0; i < sentence.size(); i++) {
      for (uint_fast32_t j = 0; j < sentence.size(); j++) {
        if (j == i) {
          in(i, j) = 1;
          if (j + 1 < sentence.size()) {
            target(i, j + 1) = 1;
          }
        }
      }
    }
    net.train(in, target, epochs);
  }
  vec<float, false> predict_next(int vocab_index) {
    mat in(1, vocab_len);
    in(0, vocab_index) = 1;
    mat out = net.forward(in);
    softmax(out);
    return out.get_row(0);
  }
  vec<float, false> get_vec(int vocab_index) { return net.get_weights(0, vocab_index); }

#ifndef CX_DELETE_TESTS
  static void TEST() {
    std::cout << "TESTING WORD2VEC" << std::endl;
    word2vec word_2_vec(4, 2);
    std::vector<string> train = {{"I"}, {"like"}, {"programming"}, {"alot"}};
    word_2_vec.train(train, 30);
    word_2_vec.predict_next(1).print();
    std::cout << word_2_vec.get_vec(1) << std::endl;
  }
#endif
};
}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_CXML_WORD2VEC_H_
