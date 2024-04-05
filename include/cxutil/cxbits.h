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
#ifndef CXSTRUCTS_SRC_CXUTIL_CXBITS_H_
#define CXSTRUCTS_SRC_CXUTIL_CXBITS_H_

#include "../cxconfig.h"
#include <cstdlib>
#include <type_traits>  //For type traits

//Assumes little endian for all operations

//// Integer Literals
//int: 42
//unsigned int: 42U
//long: 42L
//unsigned long: 42UL
//long long: 42LL
//unsigned long long: 42ULL
//octal: 052 // Equivalent to decimal 42
//hexadecimal: 0x2A // Equivalent to decimal 42
//binary: 0b101010 // Equivalent to decimal 42
//
//// Floating-Point Literals
//float: 3.14F
//double: 3.14
//long double: 3.14L
//
//// Character Literals
//char: 'A'
//wchar_t: L'A'
//char16_t: u'A'
//char32_t: U'A'
//
//// String Literals
//char string: "Hello"
//wchar_t string: L"Hello"
//char16_t string: u"Hello"
//char32_t string: U"Hello"
//UTF-8 string (C++20): u8"Hello"

namespace cxstructs {

enum Endianness { LITTLE_ENDIAN, BIG_ENDIAN };

// Type trait to check if both types are the same size and either both signed or both unsigned
template <typename T, typename U>
struct is_same_size_a_sign {
  static constexpr bool value =
      (sizeof(T) == sizeof(U)) && (std::is_signed<T>::value == std::is_signed<U>::value);
};

template <typename T, typename U, bool SameSizeAndSign>
struct ConcatType;

template <typename T, typename U>
struct ConcatType<T, U, true> {
  static constexpr size_t totalSize = sizeof(T) * 8 + sizeof(U) * 8;
  using type =
      std::conditional_t<totalSize <= 16, uint16_t,
                         std::conditional_t<totalSize <= 32, uint32_t,
                                            std::conditional_t<totalSize <= 64, uint64_t, void>>>;
};

//Concatenates two equal sized numbers
template <typename T, typename U>
inline constexpr auto bits_concat(T first, U second) ->
    typename ConcatType<T, U, is_same_size_a_sign<T, U>::value>::type {
  static_assert(std::is_integral<T>::value && std::is_integral<U>::value,
                "Only integer types are supported.");
  static_assert(is_same_size_a_sign<T, U>::value, "T and U must be of the same size and sign.");
  static_assert(
      !std::is_same_v<typename ConcatType<T, U, is_same_size_a_sign<T, U>::value>::type, void>,
      "Resulting type is too large to represent.");

  using ResultType = typename ConcatType<T, U, is_same_size_a_sign<T, U>::value>::type;
  return (static_cast<ResultType>(second) << (sizeof(T) * 8)) | static_cast<ResultType>(first);
}

//Prints the bits of the given num in the given format
template <typename T>
inline constexpr void bits_print(T num, Endianness endian = LITTLE_ENDIAN) {
  static_assert(std::is_integral<T>::value, "Only integral types are supported.");

  constexpr size_t numBits = sizeof(T) * 8;  // Total bits in type T
  char bitRepresentation[numBits + 1];       // +1 for null terminator
  bitRepresentation[numBits] = '\0';         // Null-terminate the string

  for (size_t i = 0; i < numBits; ++i) {
    size_t index = endian == LITTLE_ENDIAN ? i : (numBits - 1) - i;
    bitRepresentation[i] = (num & (T(1) << index)) ? '1' : '0';
  }

  // Print the bit representation directly
  for (size_t i = 0; i < numBits; ++i) {
    putchar(bitRepresentation[i]);
  }
  putchar('\n');
}

//Prints the bytes of the given num in the given format
template <typename T>
void bits_print_bytes(const T& num, Endianness endian = LITTLE_ENDIAN) {
  static_assert(std::is_integral<T>::value, "Only integral types are supported.");

  const size_t numBytes = sizeof(T);
  const auto* bytePointer = reinterpret_cast<const unsigned char*>(&num);

  if (endian == LITTLE_ENDIAN) {
    for (size_t i = 0; i < numBytes; ++i) {
      printf("%02X ", bytePointer[i]);
    }
  } else {
    for (size_t i = numBytes; i > 0; --i) {
      printf("%02X ", bytePointer[i - 1]);
    }
  }

  printf("\n");
}

template <typename R, typename T>
inline constexpr R bits_get(T num, uint8_t off) {
  static_assert(std::is_integral<T>::value, "Only integer type are supported.");
  return static_cast<R>(num >> off);
}


}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_CXUTIL_CXBITS_H_
