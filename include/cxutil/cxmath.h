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
#ifndef CXSTRUCTS_SRC_CXUTIL_MATH_H_
#define CXSTRUCTS_SRC_CXUTIL_MATH_H_

#include <cmath>

#define CX_PI \
  3.14159265358979323846  // for compatibility | apparently this is only in c++ through std::numbers which is CX20 and not on all compilers equal

namespace cxstructs {
struct mat;
//function pointer typedef
typedef float (*func)(float);
typedef mat (*func_M)(mat&, mat&);  // mat function
typedef float (*D_func)(float p1x, float p1y, float p2x, float p2y);

//activation functions
inline float sig(float x) noexcept {
  return 1.0F / (1.0F + std::exp(-x));
}
inline float tanh(float x) noexcept {
  return std::tanh(x);
}
inline float relu(float x) noexcept {
  return x > 0 ? x : 0;
}
//derivatives
inline float d_sig(float x) noexcept {
  return sig(x) * (1 - sig(x));
}
inline float d_relu(float x) noexcept {
  return x > 0 ? 1 : 0;
}
inline float d_tanh(float x) noexcept {
  float t = std::tanh(x);
  return 1 - t * t;
}

//utils
/**
 * Finds the <b>next</b> closest power of two to the right of the given number
 * @param n the start number
 * @return the next power of two or n, if n is a power of 2
 */
inline uint_32_cx next_power_of_2(uint_32_cx n) noexcept {
  //black magic
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  return n;
}
/**
 * Fast inverse square root from quake (inversed)
 * @param n
 * @return the square root of n
 */
inline float fast_sqrt(float n) noexcept {
  long i;
  float x2, y;
  constexpr float threehalfs = 1.5F;

  x2 = n * 0.5F;
  y = n;
  i = *(long*)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float*)&i;
  y = y * (threehalfs - (x2 * y * y));
  return 1.0F / y;
}

//distance functions
inline float euclidean(float p1x, float p1y, float p2x, float p2y) noexcept {
  return fast_sqrt((p2x - p1x) * (p2x - p1x) + (p2y - p1y) * (p2y - p1y));
}
inline float manhattan(float p1x, float p1y, float p2x, float p2y) noexcept {
  return abs(p2x - p1x) + abs(p2y - p1y);
}

}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_CXUTIL_MATH_H_
