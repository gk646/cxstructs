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
#ifndef CXSTRUCTS_SRC_CXUTIL_CXSTRING_H_
#define CXSTRUCTS_SRC_CXUTIL_CXSTRING_H_

#include "../cxconfig.h"
#include <cstdlib>
#include <type_traits>

namespace cxstructs {

template <typename T>
inline auto str_parse_token(char** context, char delim) {
  const char* start = *context;
  while (**context != '\0' && **context != delim) {
    (*context)++;
  }

  if constexpr (std::is_same_v<T, float>) {
    auto result = std::strtof(start, context);
    if (**context == delim) (*context)++;
    return result;
  } else if constexpr (std::is_same_v<T, int>) {
    auto result = (int)std::strtol(start, context, 10);
    if (**context == delim) (*context)++;
    return result;
  } else if constexpr (std::is_same_v<T, const char*>) {
    const char* result = start;
    if (**context == delim) {
      *(*context) = '\0';
      (*context)++;  // Move past the delimiter for the next call
    }
    return result;
  }
}


constexpr auto fnv1a_32(char const* s) noexcept -> uint32_t {
   uint32_t hash = 2166136261U;
  while (*s != 0) {
    hash ^= (uint32_t)(*s++);
    hash *= 16777619U;
  }
  return hash;
}

struct Fnv1aHash {
  auto operator()(const char* s) const noexcept -> std::size_t {
    uint32_t hash = 2166136261U;
    while (*s != 0) {
      hash ^= (uint32_t)(*s++);
      hash *= 16777619U;
    }
    return hash;
  }
};

#ifdef CX_INCLUDE_TESTS
static void TEST_STRING() {
  char buff[64] = "ParseME|0.3|55|-55|";
  char* ptr = buff;
  printf("%s\n", cxstructs::str_parse_token<const char*>(&ptr, '|'));
  printf("%f\n", cxstructs::str_parse_token<float>(&ptr, '|'));
  printf("%d\n", cxstructs::str_parse_token<int>(&ptr, '|'));
  printf("%d\n", cxstructs::str_parse_token<int>(&ptr, '|'));
}
#endif

}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_CXUTIL_CXSTRING_H_
