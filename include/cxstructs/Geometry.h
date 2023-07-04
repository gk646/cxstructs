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

#include <algorithm>
#include <cstdint>
#include "../cxconfig.h"

namespace cxstructs {

class Circle;
class Point;

struct Rect {
  float x_;
  float y_;
  float w_;
  float h_;

 public:
  inline Rect() : x_(0), y_(0), w_(0), h_(0) {}
  inline Rect(float x, float y, float width, float height)
      : x_(x), y_(y), w_(width), h_(height) {}
  inline Rect(float x, float y) : x_(x), y_(y), w_(0), h_(0) {}
  /**
   * @brief Checks if this rectangle intersects with another rectangle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two rectangles.
   *
   * @param r The other rectangle to check for intersection.
   * @return `true` if this rectangle intersects with the other rectangle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Rect& r) const {
    return !(x_ > r.x_ + r.w_ || x_ + w_ < r.x_ || y_ > r.y_ + r.h_ ||
             y_ + h_ < r.y_);
  }
  /**
   * @brief Checks if this rectangle intersects with a circle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param c The circle check for intersection.
   * @return `true` if this rectangle intersects with the circle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(Circle& c) const;

  [[nodiscard]] inline float& x() { return x_; }
  [[nodiscard]] inline float& y() { return y_; }
  [[nodiscard]] inline float& width() { return w_; }
  [[nodiscard]] inline float& height() { return h_; }
};

class Circle {
  float x_;
  float y_;
  float r_;

 public:
  inline Circle() : x_(0), y_(0), r_(0) {}
  inline Circle(float x_pos, float y_pos, float radius)
      : x_(x_pos), y_(y_pos), r_(radius) {}
  /**
   * @brief Checks if this circle intersects with a rectangle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param r The rect check for intersection.
   * @return `true` if this circle intersects with the rectangle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(Rect& r) const {
    float closestX = std::clamp(x_, r.x(), r.x() + r.width());
    float closestY = std::clamp(y_, r.y(), r.y() + r.height());

    float dx = closestX - x_;
    float dy = closestY - y_;

    return (dx * dx + dy * dy) <= (r_ * r_);
  }
  /**
   * @brief Checks if this circle intersects with a circle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param c The circle check for intersection.
   * @return `true` if this circle intersects with the circle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Circle& c) const {
    return !(((x_ - c.x_) * (x_ - c.x_) + (y_ - c.y_) * (y_ - c.y_)) >
             (r_ * c.r_ + r_ * c.r_));
  }
  /**
   *
   * @return  the x position
   */
  [[nodiscard]] inline float& x() { return x_; }
  /**
   *
   * @return  the y position
   */
  [[nodiscard]] inline float& y() { return y_; }
  [[nodiscard]] inline float& radius() { return r_; }
};

class Point {
  float x_;
  float y_;

 public:
  inline Point() : x_(0), y_(0) {}
  inline Point(float x_pos, float y_pos) : x_(x_pos), y_(y_pos) {}
  /**
   * Checks if the point is either inside or touches the rectangle
   *
   * @param r the rectangle
   * @return true if the point is inside or touches the rectangle
   */
  [[nodiscard]] inline bool intersects(Rect& r) const {
    return !(x_ < r.x_ || y_ < r.y_ || x_ > r.x_ + r.w_ || y_ > r.y_ + r.w_);
  }
  /**
   * Checks if the point is either inside or touches the circle
   *
   * @param r the rectangle
   * @return true if the point is inside or touches the circle
   */
  [[nodiscard]] inline bool intersects(Circle& c) const {
    return (x_ - c.x()) * (x_ - c.x()) + (y_ - c.y()) * (y_ - c.y()) <=
           (c.radius() * c.radius());
  }
  /**
   *
   * @return  the x position
   */
  [[nodiscard]] inline float& x() { return x_; }
  /**
   *
   * @return  the y position
   */
  [[nodiscard]] inline float& y() { return y_; }
};

// Definitions of member functions

bool Rect::intersects(Circle& c) const {
  float closestX = std::clamp(c.x(), x_, x_ + w_);
  float closestY = std::clamp(c.y(), y_, y_ + h_);

  float dx = closestX - c.x();
  float dy = closestY - c.y();

  return (dx * dx + dy * dy) <= (c.radius() * c.radius());
}

}  // namespace cxalgos

#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_
