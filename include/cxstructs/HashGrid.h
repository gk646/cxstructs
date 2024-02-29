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
#ifndef CXSTRUCTS_SRC_CXSTRUCTS_HASHGRID_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_HASHGRID_H_

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../cxconfig.h"

namespace cxhelper {}  // namespace cxhelper

namespace cxstructs {

/**
 * HashGrid for a square space
 */
template <typename EntityID = uint32_t>
struct HashGrid {
  using size_type = uint_32_cx;
  using GridID = size_type;

 private:
  std::unordered_map<GridID, std::vector<EntityID>> map;
  float cellSize;
  size_type spaceSize;
  size_type gridSize;

 public:
  explicit HashGrid(float cellSize, size_type spaceSize, bool reserveUpfront = true)
      : cellSize(cellSize),
        spaceSize(spaceSize),
        gridSize(static_cast<size_type>(spaceSize / cellSize)) {
    if (reserveUpfront) {
      map.reserve(gridSize);
    }
  };

  HashGrid(const HashGrid&) = delete;
  HashGrid& operator=(const HashGrid&) = delete;
  HashGrid(HashGrid&&) = delete;
  HashGrid& operator=(HashGrid&&) = delete;

  [[nodiscard]] inline GridID getGridID(float x, float y) const noexcept {
    return static_cast<int>(x / cellSize) + static_cast<int>(y / cellSize) * gridSize;
  }
  inline void clear() {
    for (auto& pair : map) {
      pair.second.clear();
    }
  };
  inline void setupNew(float newCellSize, uint_16_cx newSpaceSize, bool optimized = true) {
    if (optimized) {
      float value = cellSize / spaceSize;
      cellSize = newSpaceSize * value;
      map.reserve(std::pow(spaceSize / cellSize, 2));
    } else {
      cellSize = newCellSize;
      spaceSize = newSpaceSize;
    }
    map.clear();
  };
  inline void insert(float x, float y, EntityID entityID) {
    CX_ASSERT(x < spaceSize && y < spaceSize, "x or y is larger than spaceSize");
    map[getGridID(x, y)].emplace_back(entityID);
  };

  inline bool containedInCell(float x, float y, EntityID eID) noexcept {
    CX_ASSERT(x < spaceSize && y < spaceSize, "x or y is larger than spaceSize");
    const auto& vec = map[getGridID(x, y)];
    return std::find(vec.begin(), vec.end(), eID) != vec.end();
  }

  inline bool containedInCell(GridID gID, EntityID eID) noexcept {
    const auto& vec = map[gID];
    return std::find(vec.begin(), vec.end(), eID) != vec.end();
  }

  inline void containedInRectCollect(float x1, float y1, float x2, float y2,
                                     std::vector<EntityID>& outVec) noexcept {
    CX_ASSERT(x1 < spaceSize && y1 < spaceSize, "x or y is larger than spaceSize");
    CX_ASSERT(x2 < spaceSize && y2 < spaceSize, "x or y is larger than spaceSize");
    auto topLeftX = static_cast<size_type>(x1 / cellSize);
    auto topLeftY = static_cast<size_type>(y1 / cellSize);
    auto bottomRightX = static_cast<size_type>(x2 / cellSize);
    auto bottomRightY = static_cast<size_type>(y2 / cellSize);

    for (size_type x = topLeftX; x <= bottomRightX; ++x) {
      for (size_type y = topLeftY; y <= bottomRightY; ++y) {
        const auto& vec = map[x + y * gridSize];
        outVec.insert(outVec.end(), vec.begin(), vec.end());
      }
    }
  }


  inline bool isCloseTo(float cellRadius) {}

};

}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_CXSTRUCTS_HASHGRID_H_
