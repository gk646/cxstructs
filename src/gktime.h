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
#ifndef GK_TIME_HPP
#define GK_TIME_HPP

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace cxutil {
using namespace std; //std:: makes this code unreadable

inline chrono::time_point<chrono::high_resolution_clock> activeTimeStamp;

void static now() {
  activeTimeStamp = std::chrono::high_resolution_clock::now();
}

template <typename T>
string get_duration_unit();

template <>
string get_duration_unit<chrono::seconds>() {
  return "seconds";
}
template <>
string get_duration_unit<chrono::milliseconds>() {
  return "milliseconds";
}
template <>
string get_duration_unit<chrono::microseconds>() {
  return "microseconds";
}
template <>
string get_duration_unit<chrono::nanoseconds>() {
  return "nanoseconds";
}
template <>
string get_duration_unit<chrono::duration<double>>() {
  return "seconds";
}

template <typename durationType = chrono::duration<double>>
void printTime(string prefix = "") {
  auto diff = std::chrono::high_resolution_clock::now() - activeTimeStamp;
  auto diffInMicroseconds = chrono::duration_cast<durationType>(diff);
  if (!prefix.empty()) {
    cout << prefix << " ";
  }
  cout << fixed << setprecision(3) << diffInMicroseconds.count() << " "
       << get_duration_unit<durationType>() << endl;
}

long long getTime() {
  auto diff = chrono::high_resolution_clock::now() - activeTimeStamp;
  auto diffInMicroseconds = chrono::duration_cast<chrono::microseconds>(diff);
  cout << fixed << setprecision(9) << diffInMicroseconds.count() << endl;
  return diffInMicroseconds.count();
}

}  // namespace cxutil
#endif  //GK_TIME_HPP
