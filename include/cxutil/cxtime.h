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
#ifndef CX_TIME_H
#define CX_TIME_H

#include "../cxconfig.h"
#include <cstdio>
#include <chrono>

namespace cxstructs {
using namespace std;  //std:: makes this code unreadable

inline chrono::time_point<chrono::high_resolution_clock> activeTimeStamp;
inline chrono::time_point<chrono::high_resolution_clock> checkpoints[3];

/**
 * Sets the activeTimeStamp or alternatively the time of a checkpoint
 * @param checkpoint (optional, max=2) the checkpoint to set the current time
 */
inline void now(int checkpoint = -1) {
  if (checkpoint >= 0 && checkpoint < 3) {
    checkpoints[checkpoint] = std::chrono::high_resolution_clock::now();
  } else {
    activeTimeStamp = std::chrono::high_resolution_clock::now();
  }
}

template <typename T>
const char* get_duration_unit();
template <>
inline const char* get_duration_unit<std::chrono::seconds>() {
  return "seconds";
}
template <>
inline const char* get_duration_unit<std::chrono::milliseconds>() {
  return "milliseconds";
}
template <>
inline const char* get_duration_unit<std::chrono::microseconds>() {
  return "microseconds";
}
template <>
inline const char* get_duration_unit<std::chrono::nanoseconds>() {
  return "nanoseconds";
}
template <>
inline const char* get_duration_unit<std::chrono::duration<double>>() {
  return "seconds";
}

template <typename DurationType = std::chrono::duration<double>>
inline void printTime(const char* prefix = nullptr, int checkpoint = -1) {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  if (checkpoint >= 0 && checkpoint < 3) {
    start_time = checkpoints[checkpoint];
  } else {
    start_time = activeTimeStamp;
  }

  const auto diff = std::chrono::high_resolution_clock::now() - start_time;
  const auto diffInDesiredUnits = std::chrono::duration_cast<DurationType>(diff);

  if (prefix) {
    printf("%s ", prefix);
  }
  // Print with fixed precision of 3 decimal places
  printf("%.3f %s\n",  diffInDesiredUnits.count(), get_duration_unit<DurationType>());
}
template <typename DurationType = std::chrono::duration<double>>
inline long long getTime(int checkpoint = -1) {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  if (checkpoint >= 0 && checkpoint < 3) {
    start_time = checkpoints[checkpoint];
  } else {
    start_time = activeTimeStamp;
  }
  auto diff = std::chrono::high_resolution_clock::now() - start_time;
  auto diffInDesiredUnits = std::chrono::duration_cast<DurationType>(diff);
  return diffInDesiredUnits.count();
}

}  // namespace cxstructs
#endif  //CX_TIME_H
