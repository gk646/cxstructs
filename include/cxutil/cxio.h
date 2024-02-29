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

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace cxstructs {

/**
 * @brief Loads the entire contents of a text file into a string.
 *
 * @param filePath The path to the text file to be loaded.
 * @param contents A reference to a string where the contents of the file will be stored.
 * @return Returns true if the file was successfully loaded, false otherwise.
 */
inline bool load_txt(const std::string& filePath, std::string& contents) {

  std::ifstream file(filePath, std::ios::in);

  if (!file.is_open()) {
    std::cerr << "Could not open the file: " << filePath << std::endl;
    return false;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  contents = std::move(buffer.str());

  return true;
}

}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_CXIO_H_
