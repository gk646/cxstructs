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
  [[nodiscard]] virtual bool contains(const Point& s) const = 0;
  [[nodiscard]] virtual bool contains(const Rect& s) const = 0;
  [[nodiscard]] virtual bool contains(const Circle& s) const = 0;
  [[nodiscard]] virtual bool intersects(const Rect& r) const = 0;
  [[nodiscard]] virtual bool intersects(const Circle& r) const = 0;
};

class Rect : public Shape {
  float x_;
  float y_;
  float w_;
  float h_;

 public:
  inline Rect() : x_(0), y_(0), w_(0), h_(0) {}
  inline Rect(const float& x, const float& y, const float& width, const float& height)
      : x_(x), y_(y), w_(width), h_(height) {}
  inline Rect(const float& x, const float& y) : x_(x), y_(y), w_(0), h_(0) {}
  /**
   * @brief Checks if this rectangle intersects with another rectangle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two rectangles.
   *
   * @param r The other rectangle to check for intersection.
   * @return `true` if this rectangle contained with the other rectangle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Rect& r) const final {
    return !(x_ > r.x_ + r.w_ || x_ + w_ < r.x_ || y_ > r.y_ + r.h_ || y_ + h_ < r.y_);
  }
  /**
   * @brief Checks if this rectangle intersects with a circle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param c The circle check for intersection.
   * @return `true` if this rectangle contained with the circle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Circle& c) const final;
  /**
   * Checks if the given rect is fully contained inside this rectangle.<p>
   * Contained means non-touching
   * @param r the other rect
   * @return true only if r is fully contained
   */
  [[nodiscard]] inline bool contains(const Rect& r) const final {
    return !(x_ > r.x_ && y_ > r.y_ && x_ + w_ < r.x_ + r.w_ && y_ + h_ < r.y_ + r.h_);
  }
  [[nodiscard]] inline bool contains(const Circle& p) const final;
  [[nodiscard]] inline bool contains(const Point& p) const final;
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
  friend std::ostream& operator<<(std::ostream& os, const Rect& r) {
    os << "Rect: [x: " << r.x_ << ", y: " << r.y_ << ", width: " << r.w_ << ", height: " << r.h_
       << "]";
    return os;
  }
};

class Circle : public Shape {
  float x_;
  float y_;
  float r_;

 public:
  inline Circle() : x_(0), y_(0), r_(0) {}
  inline Circle(float x_pos, float y_pos, float radius) : x_(x_pos), y_(y_pos), r_(radius) {}
  /**
   * @brief Checks if this circle contained with a rectangle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param r The rect check for intersection.
   * @return `true` if this circle contained with the rectangle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Rect& r) const final {
    float closestX = std::clamp(x_, r.x(), r.x() + r.width());
    float closestY = std::clamp(y_, r.y(), r.y() + r.height());

    float dx = closestX - x_;
    float dy = closestY - y_;

    return (dx * dx + dy * dy) <= (r_ * r_);
  }
  /**
   * @brief Checks if this circle contained with a circle.
   *
   * An intersection is considered to occur if there is any overlap in the areas of the two shapes.
   *
   * @param c The circle check for intersection.
   * @return `true` if this circle contained with the circle, `false` otherwise.
   */
  [[nodiscard]] inline bool intersects(const Circle& c) const final {
    return !(((x_ - c.x_) * (x_ - c.x_) + (y_ - c.y_) * (y_ - c.y_)) > (r_ * c.r_ + r_ * c.r_));
  }
  /**
   * Checks if the given circle is fully contained inside this circle.<p>
   * Contained means non-touching
   * @param c the other circle
   * @return true only if c is fully contained
   */
  [[nodiscard]] inline bool contains(const Circle& c) const final {
    return ((x_ - c.x_) * (x_ - c.x_) + (y_ - c.y_) * (y_ - c.y_)) < (r_ - c.r_) * (r_ - c.r_);
  }
  [[nodiscard]] bool contains(const Rect& r) const final {
    float dx = std::max(0.0f, std::max(r.x() - x_, x_ - (r.x() + r.width())));
    float dy = std::max(0.0f, std::max(r.y() - y_, y_ - (r.y() + r.height())));

    return (dx * dx + dy * dy) <= (r_ * r_);
  }
  [[nodiscard]] inline bool contains(const Point& p) const final;
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
  inline Point(const float& x_pos, const float& y_pos) : x_(x_pos), y_(y_pos) {}
  inline Point& operator=(const Point& o) {
    if (this != &o) {
      x_ = o.x_;
      y_ = o.y_;
    }
    return *this;
  }
  inline Point(const Point& o) : x_(o.x_), y_(o.y_) {}
  inline bool operator==(const Point& o) const { return (x_ == o.x_ && y_ == o.y_); }
  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "Point:[" << p.x_ << " ," << p.y_ << "]";
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
    return cxalgos::fast_sqrt((p.x_ - x_) * (p.x_ - x_) + (p.y_ - y_) * (p.y_ - y_));
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

  return ((closestX - c.x()) * (closestX - c.x()) + (closestY - c.y()) * (closestY - c.y())) <=
         (c.radius() * c.radius());
}
bool Rect::contains(const Point& p) const {
  return !(x_ > p.x() || y_ > p.y() || x_ + w_ < p.x() || y_ + h_ < p.y());
}
bool Rect::contains(const Circle& p) const {
  float dx = std::max(0.0f, std::max(x_ - p.x(), p.x() - (x_ + w_)));
  float dy = std::max(0.0f, std::max(y_ - p.y(), p.y() - (y_ + h_)));

  return (dx * dx + dy * dy) > (p.radius() * p.radius());
}
bool Circle::contains(const Point& p) const {
  return ((x_ - p.x()) * (x_ - p.x()) + (y_ - p.y()) * (y_ - p.y()) < r_ * r_);
}

}  // namespace cxstructs
namespace std {
template <>
struct hash<cxstructs::Point> {
  std::size_t operator()(const cxstructs::Point& p) const {
    return static_cast<int>(p.x()) ^ (static_cast<int>(p.y()) << 1);
  }
};
template <>
struct hash<cxstructs::Rect> {
  std::size_t operator()(const cxstructs::Rect& r) const {
    return static_cast<int>(r.x()) ^ (static_cast<int>(r.y()) << 1) ^ static_cast<int>(r.width()) ^
           (static_cast<int>(r.height()) << 1);
  }
};
template <>
struct hash<cxstructs::Circle> {
  std::size_t operator()(const cxstructs::Circle& c) const {
    return static_cast<int>(c.x()) ^ (static_cast<int>(c.y()) << 1) ^ static_cast<int>(c.radius());
  }
};
}  // namespace std
#ifndef CX_DELETE_TESTS
static void GEOMETRY_TEST() {
  using namespace cxstructs;
  std::cout << "RECTANGLE TESTS" << std::endl;
  // Rectangles that are the same
  std::cout << "  Testing rectangles that are the same..." << std::endl;
  Rect r1(10, 10, 50, 50);
  Rect r2(10, 10, 50, 50);
  CX_ASSERT(r1.intersects(r2), "Same rectangles should intersect");
  CX_ASSERT(r1.contains(r2));
  // Rectangles that share an edge
  std::cout << "  Testing rectangles that share an edge..." << std::endl;
  Rect r3(10, 10, 50, 50);
  Rect r4(60, 10, 50, 50);
  CX_ASSERT(r3.intersects(r4), "Rectangles that share an edge should intersect");

  // Rectangle inside another rectangle
  std::cout << "  Testing rectangle inside another rectangle..." << std::endl;
  Rect r5(10, 10, 50, 50);
  Rect r6(20, 20, 10, 10);
  CX_ASSERT(r5.intersects(r6), "Rectangle inside another rectangle should intersect");

  // Rectangles with intersecting area
  std::cout << "  Testing rectangles with intersecting area..." << std::endl;
  Rect r7(10, 10, 50, 50);
  Rect r8(40, 40, 50, 50);
  CX_ASSERT(r7.intersects(r8), "Overlapping rectangles should intersect");

  // Non-intersecting rectangles
  std::cout << "  Testing non-intersecting rectangles..." << std::endl;
  Rect r9(10, 10, 50, 50);
  Rect r10(100, 100, 50, 50);
  CX_ASSERT(!r9.intersects(r10), "Non-overlapping rectangles should not intersect");

  std::cout << "  Testing circle and rectangle intersection...\n";

  // Circle inside rectangle
  Circle c1(5, 5, 1);
  Rect r11(0, 0, 10, 10);
  CX_ASSERT(r11.intersects(c1), "Circle inside rectangle should intersect");

  // Circle intersecting rectangle at edge
  Circle c2(10, 5, 5);
  Rect r12(0, 0, 10, 10);
  CX_ASSERT(r12.intersects(c2), "Circle intersecting rectangle at edge should intersect");

  // Circle not intersecting rectangle
  Circle c3(15, 15, 1);
  Rect r13(0, 0, 10, 10);
  CX_ASSERT(!r13.intersects(c3), "Circle not intersecting rectangle should not intersect");

  // Circle intersecting rectangle at corner
  Circle c4(10, 10, 5);
  Rect r14(0, 0, 10, 10);
  CX_ASSERT(r14.intersects(c4), "Circle intersecting rectangle at corner should intersect");

  std::cout << "  Testing rectangle contained...\n";
  Rect r16(1, 1, 8, 8);
  Point p10(5, 5);
  Point p11(9, 9);
  CX_ASSERT(r14.contains(r16), "Rectangle should contain smaller rectangle");
  CX_ASSERT(r14.contains(r14), "Rectangle should not contain itself");
  CX_ASSERT(r16.contains(p10), "Rectangle should contain point inside it");
  CX_ASSERT(r14.contains(p11), "Rectangle should contain point on its edge");
  CX_ASSERT(r16.contains(p11));

  std::cout << "  Testing circle contained circle...\n";
  Circle c5(0.0, 0.0, 10.0);
  Circle c6(8.0, 0.0, 5.0);   // Circle contained within c5
  Circle c7(20.0, 0.0, 5.0);  // Circle outside c5
  Circle c10(0.0, 0.0, 5.0);
  Circle c11(0.0, 0.0, 10.0);

  // Circle c6 contained within Circle c5
  CX_ASSERT(c6.intersects(c5), "Circle contained within another circle should intersect");

  // Circle c7 does not intersect with Circle c5
  CX_ASSERT(!c7.intersects(c5), "Circle not intersecting another circle should not intersect");

  // Circle c10 contained within Circle c5
  CX_ASSERT(c5.contains(c10), "Circle c10 should be contained within Circle c5");

  // Circle c7 not contained within Circle c5
  CX_ASSERT(!c5.contains(c7), "Circle c7 should not be contained within Circle c5");

  // Circle c11 not contained within Circle c5
  CX_ASSERT(!c5.contains(c11), "Circle c11 should not be contained within Circle c5");

  std::cout << "POINT TESTS:" << std::endl;

  // Define some rectangles
  Rect r15(0.0, 0.0, 10.0, 10.0);

  // Define some points
  Point p1(5.0, 5.0);
  Point p2(15.0, 5.0);

  // Point p1 is inside Rectangle r15

  // Define some circles
  Circle c8(0.0, 0.0, 10.0);

  // Define some points
  Point p3(5.0, 5.0);
  Point p4(15.0, 5.0);

  // Point p3 is inside Circle c8
}
#endif
#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_
