#ifndef GK_TIME_HPP
#define GK_TIME_HPP

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace gkutils {
using namespace std;

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

}  // namespace gkutils
#endif  //GK_TIME_HPP
