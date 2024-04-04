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
#include <cmath>
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
  float cellSize;
  float spaceSize;
  size_type gridSize;
  std::unordered_map<GridID, std::vector<EntityID>> map;

 public:
  explicit HashGrid(float cellSize, float spaceSize, bool reserveUpfront = true)
      : cellSize(cellSize),
        spaceSize(spaceSize),
        gridSize(static_cast<size_type>(spaceSize / cellSize)) {
    map.reserve(gridSize * gridSize);
    map.max_load_factor(1.0F);
  };
  HashGrid() = default;
  HashGrid(const HashGrid&) = delete;
  HashGrid& operator=(const HashGrid&) = delete;
  HashGrid(HashGrid&&) = delete;
  HashGrid& operator=(HashGrid&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    map = std::move(other.map);
    cellSize = other.cellSize;
    spaceSize = other.spaceSize;
    gridSize = other.gridSize;

    other.cellSize = 0;
    other.spaceSize = 0;
    other.gridSize = 0;

    return *this;
  }
  inline const std::vector<EntityID>& operator[](GridID g) noexcept { return map[g]; }
  [[nodiscard]] inline GridID getGridID(float x, float y) const noexcept {
    return static_cast<int>(x / cellSize) + static_cast<int>(y / cellSize) * gridSize;
  }
  inline void getGridIDs(int32_t (&gridIDs)[4], float x, float y, float width,
                         float height) const noexcept {
    int topLeftGridX = static_cast<int>(x / cellSize);
    int topLeftGridY = static_cast<int>(y / cellSize);
    int bottomRightGridX = static_cast<int>((x + width) / cellSize);
    int bottomRightGridY = static_cast<int>((y + height) / cellSize);

    std::fill(std::begin(gridIDs), std::end(gridIDs), -1);

    gridIDs[0] = topLeftGridX + topLeftGridY * gridSize;
    gridIDs[1] = (topLeftGridX != bottomRightGridX) ? bottomRightGridX + topLeftGridY * gridSize
                                                    : -1;  // Top-right
    gridIDs[2] = (topLeftGridY != bottomRightGridY) ? topLeftGridX + bottomRightGridY * gridSize
                                                    : -1;  // Bottom-left
    gridIDs[3] = (topLeftGridX != bottomRightGridX && topLeftGridY != bottomRightGridY)
                     ? bottomRightGridX + bottomRightGridY * gridSize
                     : -1;  // Bottom-right
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
};

}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_CXSTRUCTS_HASHGRID_H_
