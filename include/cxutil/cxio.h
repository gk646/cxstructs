// Copyright (c) 2023 gk646
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
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
#include <cstring>

// Simple, readable and fast *symmetric* serialization structure with loading
// and saving. Each line is a concatenated list of values and delimiter '|'
// (might be changeable in the future) 13|3.145|This is a string|56|

// Using the CXX23 std::print() is about 10% slower
namespace cxstructs {
// inline static constexpr char DELIMITER = '|'; //Not used yet
static constexpr int MAX_SECTION_SIZE = 32;

//-----------SHARED-----------//
namespace {
// Dont wanna include <string>
int manual_strlen(const char *str) {
  int len = 0;
  while (str[len] != '\0')
    len++;
  return len;
}
int manual_strncmp(const char *s1, const char *s2, int n) {
  for (int i = 0; i < n && s1[i] && s2[i]; i++) {
    if (s1[i] != s2[i])
      return s1[i] - s2[i];
  }
  return 0;
}
} // namespace
// true if end of file // don't call this more than once per line
inline auto io_check_eof(FILE *file) -> bool {
  // Remember the current position
  long currentPos = ftell(file);
  if (currentPos == -1)
    return true; // Error in ftell

  // Try to read a byte
  char ch;
  if (fread(&ch, 1, 1, file) != 1) {
    return true; // EOF reached or read error
  }

  // Seek back to the original position
  fseek(file, currentPos, SEEK_SET);
  return false; // Not EOF
}

//-----------SAVING-----------//
// Writes a section header - used like: while(io_load_inside_section()){}
inline void io_save_section(FILE *file, const char *value) {
  fprintf(file, "--%s--\n", value);
}
// Writes a new line to file
inline void io_save_newline(FILE *file) { fputc('\n', file); }
// Writes a string value to file
inline void io_save(FILE *file, const char *value) {
  fprintf(file, "%s|", value);
}
// Writes an integer or enum property to the file
inline void io_save(FILE *file, const int value) {
  fprintf(file, "%d|", value);
}
// Writes a float property to the file
inline void io_save(FILE *file, const float value) {
  fprintf(file, "%.3f|", value);
}
// Buffers the given SaveFunc to memory so the file is only written if it
// executes successfully. Returns false on error
template <typename SaveFunc> // SaveFunc(FILE* file)
bool io_save_buffered_write(const char *fileName, const int memoryBufferBytes,
                            SaveFunc func) {
  FILE *file;

  // Write to in memory buffer
  if (fopen_s(&file, "NUL", "w") != 0) {
    return false;
  }

  auto *buffer = new char[memoryBufferBytes];
  std::memset(buffer, 0, memoryBufferBytes);

  // _IOLBF is line buffering
  if (setvbuf(file, buffer, _IOLBF, memoryBufferBytes) != 0) {
    delete[] buffer;
    return false;
  }

  // Call the user function
  func(file);

  if (fclose(file) != 0) {
    delete[] buffer;
    return false;
  }

  // When successful, open the actual save file and save the data
  const int dataSize = (int)strlen(buffer);
  if (fopen_s(&file, fileName, "w") != 0) {
    delete[] buffer;
    return false;
  }

  // Set no buffering for more efficiency
  if (setvbuf(file, nullptr, _IONBF, 0)) {
    delete[] buffer;
    return false;
  }

  fwrite(buffer, dataSize, 1, file);

  delete[] buffer;

  if (fclose(file) != 0) {
    return false;
  }

  return true;
}
//-----------LOADING-----------//
// Searches for the next new line but stops at the delimiter if not forced
inline void io_load_newline(FILE *file, bool force = false) {
  char ch;
  while (fread(&ch, 1, 1, file) == 1) {
    if (!force && ch == '|')
      return;
    if (ch == '\n')
      return;
  }
}
inline bool io_load_inside_section(FILE *file, const char *section) {
  long currentPos = ftell(file);
  if (currentPos == -1)
    return false; // Error - we return false

  char ch;
  if (fread(&ch, 1, 1, file) != 1) {
    fseek(file, currentPos, SEEK_SET);
    return false;
  }

  if (ch == '-') {
    if (fread(&ch, 1, 1, file) != 1 || ch != '-') {
      fseek(file, currentPos, SEEK_SET);
      return true; // Not a new section marker
    }

    char buffer[MAX_SECTION_SIZE] = {0};
    int count = 0;
    int sectionLength = manual_strlen(section);
    while (fread(&ch, 1, 1, file) == 1 && count < sectionLength &&
           count < MAX_SECTION_SIZE - 1) {
      buffer[count++] = ch;
    }
    buffer[count] = '\0'; // Null-terminate the string
    if (manual_strncmp(buffer, section, sectionLength) == 0) {
      io_load_newline(file, false);
      return true; // Found same section
    } else {
      io_load_newline(file, false);
      return false; // Found new section
    }
  }

  fseek(file, currentPos, SEEK_SET);
  return true; // Still inside same section
}
// include <string> to use
#ifdef _STRING_
inline void io_load(FILE *file, std::string &s, int reserve_amount = 50) {
  s.reserve(reserve_amount);
  char ch;
  while (fread(&ch, 1, 1, file) == 1 && ch != '|') {
    s.push_back(ch);
  }
}
#endif
// Load a string property into a user-supplied buffer
inline void io_load(FILE *file, char *buffer, size_t buffer_size) {
  size_t count = 0;
  char ch;
  while (count < buffer_size - 1 && fread(&ch, 1, 1, file) == 1 && ch != '|') {
    buffer[count++] = ch;
  }
  buffer[count] = '\0';
}
// Directly load an integer property from the file
inline void io_load(FILE *file, int &i) { fscanf_s(file, "%d|", &i); }
// Directly load a float property from the file
inline void io_load(FILE *file, float &f) { fscanf_s(file, "%f|", &f); }

} // namespace cxstructs

#endif // CXSTRUCTS_SRC_CXIO_H_