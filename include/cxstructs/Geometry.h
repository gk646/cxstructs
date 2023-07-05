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
#include "../cxalgos/MathFunctions.h"
#include "../cxconfig.h"

namespace cxstructs {

class Point;
class Circle;
class Rect;

class Shape {

 public:
  [[nodiscard]] virtual bool contains(const Shape& s) const {};
  [[nodiscard]] virtual bool intersects(const Shape& r) const {};
};

class Rect : public Shape {
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
   * @brief Checks if this rectangle contains with another rectangle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two rectangles.
   *
   * @param r The other rectangle to check for intersection.
   * @return `true` if this rectangle contains with the other rectangle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Rect& r) const {
    return !(x_ > r.x_ + r.w_ || x_ + w_ < r.x_ || y_ > r.y_ + r.h_ ||
             y_ + h_ < r.y_);
  }
  /**
   * @brief Checks if this rectangle contains with a circle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param c The circle check for intersection.
   * @return `true` if this rectangle contains with the circle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Circle& c) const;
  /**
   * Checks if the given rect is fully contains inside this rectangle.<p>
   * Contained means non-touching
   * @param r the other rect
   * @return true only if r is fully contains
   */
  [[nodiscard]] inline bool contains(const Rect& r) const {
    return (x_ < r.x_ && y_ < r.y_ && x_ + w_ > r.x_ + r.w_ &&
            y_ + h_ > r.y_ + r.h_);
  }
  [[nodiscard]] inline bool contains(const Circle& p) const;
  [[nodiscard]] inline bool contains(const Point& p) const;
  /**
 * @brief Getter method for the x position.
 * @return A readable/writable reference to the x position.
 */
  [[nodiscard]] inline float& x() { return x_; }

  /**
 * @brief Getter method for the y position.
 * @return A readable/writable reference to the y position.
 */
  [[nodiscard]] inline float& y() { return y_; }

  /**
 * @brief Const getter method for the x position.
 * @return The x position. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float x() const { return x_; }

  /**
 * @brief Const getter method for the y position.
 * @return The y position. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float y() const { return y_; }
  [[nodiscard]] inline float width() const { return w_; }
  [[nodiscard]] inline float height() const { return h_; }
  [[nodiscard]] inline float& width() { return w_; }
  [[nodiscard]] inline float& height() { return h_; }
};

class Circle : public Shape {
  float x_;
  float y_;
  float r_;

 public:
  inline Circle() : x_(0), y_(0), r_(0) {}
  inline Circle(float x_pos, float y_pos, float radius)
      : x_(x_pos), y_(y_pos), r_(radius) {}
  /**
   * @brief Checks if this circle contains with a rectangle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param r The rect check for intersection.
   * @return `true` if this circle contains with the rectangle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Rect& r) const {
    float closestX = std::clamp(x_, r.x(), r.x() + r.width());
    float closestY = std::clamp(y_, r.y(), r.y() + r.height());

    float dx = closestX - x_;
    float dy = closestY - y_;

    return (dx * dx + dy * dy) <= (r_ * r_);
  }
  /**
   * @brief Checks if this circle contains with a circle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param c The circle check for intersection.
   * @return `true` if this circle contains with the circle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Circle& c) const {
    return !(((x_ - c.x_) * (x_ - c.x_) + (y_ - c.y_) * (y_ - c.y_)) >
             (r_ * c.r_ + r_ * c.r_));
  }
  /**
   * Checks if the given circle is fully contains inside this circle.<p>
   * Contained means non-touching
   * @param c the other circle
   * @return true only if c is fully contains
   */
  [[nodiscard]] inline bool contains(const Circle& c) const {
    return ((x_ - c.x_) * (x_ - c.x_) + (y_ - c.y_) * (y_ - c.y_)) <
           (r_ - c.r_) * (r_ - c.r_);
  }
  [[nodiscard]] bool contains(const Rect& r) const {
    float dx = std::max(0.0f, std::max(r.x() - x_, x_ - (r.x() + r.width())));
    float dy = std::max(0.0f, std::max(r.y() - y_, y_ - (r.y() + r.height())));

    return (dx * dx + dy * dy) <= (r_ * r_);
  }
  inline bool contains(const Point& p) const;
  /**
 * @brief Getter method for the x position.
 * @return A readable/writable reference to the x position.
 */
  [[nodiscard]] inline float& x() { return x_; }

  /**
 * @brief Getter method for the y position.
 * @return A readable/writable reference to the y position.
 */
  [[nodiscard]] inline float& y() { return y_; }

  /**
 * @brief Const getter method for the x position.
 * @return The x position. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float x() const { return x_; }

  /**
 * @brief Const getter method for the y position.
 * @return The y position. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float y() const { return y_; }
  /**
 * @brief Const getter method for the radius.
 * @return The radius. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float radius() const { return r_; }
  /**
 * @brief getter method for the radius.
 * @return A readable/writable reference to the radius.
 */
  [[nodiscard]] inline float& radius() { return r_; }
};

class Point {
  float x_;
  float y_;

 public:
  inline Point() : x_(0), y_(0) {}
  inline Point(float x_pos, float y_pos) : x_(x_pos), y_(y_pos) {}
  inline Point& operator=(const Point& o) {
    if(this!= &o){
      x_ = o.x_;
      y_ = o.y_;
    }
    return *this;
  }
  inline Point(const Point& o) : x_(o.x_), y_(o.y_) {}
  inline bool operator==(const Point& o) const {
    return (x_ == o.x_ && y_ == o.y_);
  }
  friend std::ostream& operator<<(std::ostream& os, const Point& p){
    return os<<"Point:["<<p.x_<<" ,"<<p.y_<<"]";
  }
  /**
   * Checks if the point is either inside or touches the rectangle
   *
   * @param r the rectangle
   * @return true if the point is inside or touches the rectangle
   */
  [[nodiscard]] inline bool contains(Rect& r) const {
    return (x_ > r.x() && y_ > r.y() && x_ < r.x() + r.width() &&
            y_ < r.y() + r.height());
  }
  /**
   * Checks if the point is either inside or touches the circle
   *
   * @param r the rectangle
   * @return true if the point is inside or touches the circle
   */
  [[nodiscard]] inline bool contains(Circle& c) const {
    return (x_ - c.x()) * (x_ - c.x()) + (y_ - c.y()) * (y_ - c.y()) <=
           (c.radius() * c.radius());
  }
  bool operator==(const Rect& r) const { return (x_ == r.x() && y_ == r.y()); }
  /**
   * Calculates the squared distance from this to the given point
   * @param p to other point
   * @return the squared distance
   */
  [[nodiscard]] float dist_sqr(const Point& p) const {
    return (p.x_ - x_) * (p.x_ - x_) + (p.y_ - y_) * (p.y_ - y_);
  }
  /**
   * Calculates the distance from this to the given point
   * @param p to other point
   * @return the  distance
   */
  [[nodiscard]] float dist(const Point& p) const {
    return cxalgos::fast_sqrt((p.x_ - x_) * (p.x_ - x_) +
                              (p.y_ - y_) * (p.y_ - y_));
  }
  [[nodiscard]] float dist(const float& x, const float& y) const {
    return cxalgos::fast_sqrt((x - x_) * (x - x_) + (y - y_) * (y - y_));
  }
  /**
 * @brief Getter method for the x position.
 * @return A readable/writable reference to the x position.
 */
  [[nodiscard]] inline float& x() { return x_; }

  /**
 * @brief Getter method for the y position.
 * @return A readable/writable reference to the y position.
 */
  [[nodiscard]] inline float& y() { return y_; }

  /**
 * @brief Const getter method for the x position.
 * @return The x position. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float x() const { return x_; }

  /**
 * @brief Const getter method for the y position.
 * @return The y position. Used when object is const, and does not allow modification.
 */
  [[nodiscard]] inline float y() const { return y_; }
};

// Definitions of member functions
bool Rect::intersects(const Circle& c) const {
  const float closestX = std::clamp(c.x(), x_, x_ + w_);
  const float closestY = std::clamp(c.y(), y_, y_ + h_);

  return ((closestX - c.x()) * (closestX - c.x()) +
          (closestY - c.y()) * (closestY - c.y())) <= (c.radius() * c.radius());
}
bool Rect::contains(const Point& p) const {
  return !(x_ > p.x() && y_ > p.y() && x_ + w_ < p.x() && y_ + h_ < p.y());
}
bool Rect::contains(const Circle& p) const {
  float dx = std::max(0.0f, std::max(x_ - p.x(), p.x() - (x_ + w_)));
  float dy = std::max(0.0f, std::max(y_ - p.y(), p.y() - (y_ + h_)));

  return (dx * dx + dy * dy) > (p.radius() * p.radius());
}
bool Circle::contains(const Point& p) const {
  return ((x_ - p.x()) * (x_ - p.x()) + (y_ - p.y()) * (y_ - p.y())) < r_ * r_;
}

}  // namespace cxstructs

#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_
