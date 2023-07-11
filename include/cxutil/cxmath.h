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
#ifndef CXSTRUCTS_SRC_CXUTIL_MATH_H_
#define CXSTRUCTS_SRC_CXUTIL_MATH_H_

#define CX_PI 3.14159265358979323846  // for compatibility

namespace cxutil {
inline float sig(float x) noexcept {
  return 1.0 / (1.0 + std::exp(-x));
}
inline float d_sig(float x) noexcept {
  return sig(x) * (1 - sig(x));
}

inline float relu(float x) noexcept {
  return x > 0 ? x : 0;
}
inline float d_relu(float x) noexcept {
  return x > 0 ? 1 : 0;
}
inline float tanh(float x) noexcept {
  return std::tanh(x);
}
inline float d_tanh(float x) noexcept {
  float t = std::tanh(x);
  return 1 - t * t;
}



}  // namespace cxutil
#endif  //CXSTRUCTS_SRC_CXUTIL_MATH_H_
