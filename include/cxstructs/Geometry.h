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
#ifndef CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_
#define CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_

#include "../cxconfig.h"
#include <algorithm>
#include <cstdint>

namespace cxstructs {

class circle;

struct rect {
  float x_;
  float y_;
  float w_;
  float h_;

 public:
  rect() : x_(0), y_(0), w_(0), h_(0) {}
  rect(float x, float y, float width, float height)
      : x_(x), y_(y), w_(width), h_(height) {}
  rect(float x, float y) : x_(x), y_(y), w_(0), h_(0) {}

  [[nodiscard]] inline bool intersects(const rect& o) const {
    return !(x_ > o.x_ + o.w_ || x_ + w_ < o.x_ || y_ > o.y_ + o.h_ ||
             y_ + h_ < o.y_);
  }
  [[nodiscard]] inline bool intersects(circle& c) const;
  [[nodiscard]] inline float& x() { return x_; }
  [[nodiscard]] inline float& y() { return y_; }
  [[nodiscard]] inline float& width() { return w_; }
  [[nodiscard]] inline float& height() { return h_; }
};

class circle {
  float x_;
  float y_;
  float r_;

 public:
  circle() : x_(0), y_(0), r_(0) {}
  circle(float x, float y, float r)
      : x_(x), y_(y), r_(r) {}
  [[nodiscard]] inline bool intersects(rect& r) const {
    float closestX = std::clamp(x_, r.x(), r.x() + r.width());
    float closestY = std::clamp(y_, r.y(), r.y() + r.height());

    float dx = closestX - x_;
    float dy = closestY - y_;

    return (dx * dx + dy * dy) <= (r_ * r_);
  }
  [[nodiscard]] inline bool intersects(const circle& c) const {
    return  !(((c.x_-x_) * (c.x_-x_) - (c.x_-x_) * (c.x_-x_)) > r_);
  }
  [[nodiscard]] inline float& x() { return x_; }
  [[nodiscard]] inline float& y() { return y_; }
  [[nodiscard]] inline float& radius() { return r_; }
};

// Definitions of member functions

bool rect::intersects(circle& c) const {
  float closestX = std::clamp(c.x(), x_, x_ + w_);
  float closestY = std::clamp(c.y(), y_, y_ + h_);

  float dx = closestX - c.x();
  float dy = closestY - c.y();

  return (dx * dx + dy * dy) <= (c.radius() * c.radius());
}

}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_
