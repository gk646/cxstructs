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


#ifndef CXSTRUCTS_SRC_CONFIG_H_
#define CXSTRUCTS_SRC_CONFIG_H_
/**
 * namespace for all utility related headers
 */
namespace cxutil{}
/**
 * namespace for all machinelearning related headers
 */
namespace cxml{}
/**
 * namespace for all datastructure related headers
 */
namespace cxstructs{}
#include <cstdint>
#include "cxassert.h"

typedef uint_fast32_t uint_32_cx;
typedef uint_fast16_t uint_16_cx;
typedef int_fast32_t int_32_cx;

#ifdef CX_ALLOC
#include "CXAllocator.h"
#endif

#endif  //CXSTRUCTS_SRC_CONFIG_H_
