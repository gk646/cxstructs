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
#ifndef CX_TIME_H
#define CX_TIME_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "../cxconfig.h"

namespace cxstructs {
using namespace std;  //std:: makes this code unreadable

static inline chrono::time_point<chrono::high_resolution_clock> activeTimeStamp;
static inline chrono::time_point<chrono::high_resolution_clock> checkpoints[2];
/**
 * Sets the activeTimeStamp or alternatively the time of a checkpoint
 * @param checkpoint (optional, max=1) the checkpoint to set the current time
 */
inline void static now(int_32_cx checkpoint = -1) {
  if (checkpoint > -1) {
    checkpoints[checkpoint] = chrono::high_resolution_clock::now();
    return;
  }
  activeTimeStamp = chrono::high_resolution_clock::now();
}

template <typename T>
string get_duration_unit();

template <>
inline string get_duration_unit<chrono::seconds>() {
  return "seconds";
}
template <>
inline string get_duration_unit<chrono::milliseconds>() {
  return "milliseconds";
}
template <>
inline string get_duration_unit<chrono::microseconds>() {
  return "microseconds";
}
template <>
inline string get_duration_unit<chrono::nanoseconds>() {
  return "nanoseconds";
}
template <>
inline string get_duration_unit<chrono::duration<double>>() {
  return "seconds";
}

template <typename DurationType = std::chrono::duration<double>>
inline void printTime(const std::string& prefix = "", const int32_t& checkpoint = -1) {
  using namespace std::chrono;
  using std::cout;
  using std::endl;
  using std::fixed;
  using std::setprecision;

  time_point<high_resolution_clock> start_time;
  if (checkpoint > -1) {
    start_time = checkpoints[checkpoint];
  } else {
    start_time = activeTimeStamp;
  }

  auto diff = high_resolution_clock::now() - start_time;
  auto diffInDesiredUnits = duration_cast<DurationType>(diff);

  if (!prefix.empty()) {
    cout << prefix << " ";
  }
  cout << fixed << setprecision(3) << diffInDesiredUnits.count() << " "
       << get_duration_unit<DurationType>() << endl;
}

template <typename durationType = chrono::duration<double>>
inline long long getTime() {
  auto diff = std::chrono::high_resolution_clock::now() - activeTimeStamp;
  auto diffInMicroseconds = chrono::duration_cast<durationType>(diff);
  return diffInMicroseconds.count();
}

}  // namespace cxstructs
#endif  //CX_TIME_HPP
