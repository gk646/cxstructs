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
#ifndef CXSTRUCTS_SRC_ALGORITHMS_MATHFUNCTIONS_H_
#define CXSTRUCTS_SRC_ALGORITHMS_MATHFUNCTIONS_H_

#include <cmath>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace cxalgos {
constexpr static inline double PI = 3.14159265358979323846;
/**
 * @brief Approximates the definite integral of a function over a given interval.
 *
 * @tparam Function A callable object (e.g. function, lambda) that takes a double as an argument and returns a double.
 * @param fx The function to be integrated.
 * @param a The start of the interval over which to integrate.
 * @param b The end of the interval over which to integrate.
 * @param steps The number of steps to use in the approximation (optional, default is 100 mil).
 * @return The approximate value of the integral over the specified interval.
 */
template <typename Function,
          typename =
              std::enable_if_t<std::is_invocable_r_v<double, Function, double>>>
double integral_aprox(Function fx, double a, double b,
                      uint_32_cx steps = 100000000) {
  double width = b - a;
  double step_size = width / steps;
  double integral = 0;
  for (uint_32_cx i = 0; i < steps; i++) {
    integral += fx(a) * step_size;
    a += step_size;
  }
  return integral;
}
}  // namespace cxalgos

namespace cxtests {
using namespace cxalgos;
static void TEST_MATH() {

  auto a = integral_aprox([](double x) { return x * x; }, 0, 5);
  std::cout << a << std::endl;
  a = integral_aprox(
      [](double x) { return std::pow(std::sin(x), 4) * std::cos(x); }, 0,
      PI / 2,100000);
  std::cout << a << std::endl;
}
}  // namespace cxtests
#endif  //CXSTRUCTS_SRC_ALGORITHMS_MATHFUNCTIONS_H_
