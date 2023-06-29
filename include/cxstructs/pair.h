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
#ifndef CXSTRUCTS_SRC_DATASTRUCTURES_PAIR_H_
#define CXSTRUCTS_SRC_DATASTRUCTURES_PAIR_H_

#include "../cxconfig.h"

template <typename T1_, typename T2_>
struct pair {
  bool assigned;
  T1_ first_{};
  T2_ second_{};
  pair(): assigned(false){}
  pair(const T1_ first, const T2_ second) : first_(first), second_(second) , assigned(true) {}
};

#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_PAIR_H_
