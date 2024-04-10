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
#ifndef CXSTRUCTS_SRC_CXIO_H_
#define CXSTRUCTS_SRC_CXIO_H_

#include "../cxconfig.h"

//Simple, readable and fast *symmetric* serialization structure with loading and saving
//Each line is a concatenated list of values and delimiter '|' (might be changeable in the future)
//13|3.145|This is a string|56|

//Using the CXX23 std::print() is about 10% slower
namespace cxstructs {
//inline static constexpr char DELIMITER = '|'; //Not used yet

//-----------SAVING-----------//
// Writes a new line to file
inline void io_save_newline(FILE* file) {
  fputc('\n', file);
}
// Writes a string value to file
inline void io_save(FILE* file, const char* value) {
  fprintf(file, "%s|", value);
}
// Writes an integer or enum property to the file
inline void io_save(FILE* file, int value) {
  fprintf(file, "%d|", value);
}
// Writes a float property to the file
inline void io_save(FILE* file, float value) {
  fprintf(file, "%.3f|", value);
}

//-----------LOADING-----------//
//Attempts to skip a new line until the next delimiter
inline void io_load_newline(FILE* file) {
  char ch;
  while (fread(&ch, 1, 1, file) == 1 && ch != '|') {
    if (ch == '\n') return;
  }
}
//include <string> to use or define _STRING_
#ifdef _STRING_
inline void io_load(FILE* file, std::string& s, int reserve_amount = 50) {
  s.reserve(reserve_amount);
  char ch;
  while (fread(&ch, 1, 1, file) == 1 && ch != '|') {
    s.push_back(ch);
  }
}
#endif
// Load a string property into a user-supplied buffer
inline void io_load(FILE* file, char* buffer, size_t buffer_size) {
  size_t count = 0;
  char ch;
  while (count < buffer_size - 1 && fread(&ch, 1, 1, file) == 1 && ch != '|') {
    buffer[count++] = ch;
  }
  buffer[count] = '\0';
}
// Directly load an integer property from the file
inline void io_load(FILE* file, int& i) {
  fscanf_s(file, "%d|", &i);
}
// Directly load a float property from the file
inline void io_load(FILE* file, float& f) {
  fscanf_s(file, "%f|", &f);
}

}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_CXIO_H_
