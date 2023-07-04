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

#ifndef CX_DELETE_TESTS
static void GEOMETRY_TEST() {
  using namespace cxstructs;
  std::cout << "RECTANGLE TESTS" << std::endl;
  // Rectangles that are the same
  Rect r1(10, 10, 50, 50);
  Rect r2(10, 10, 50, 50);
  CX_ASSERT(r1.intersects(r2), "Test 1: Same rectangles should intersect");

  // Rectangles that share an edge
  Rect r3(10, 10, 50, 50);
  Rect r4(60, 10, 50, 50);
  CX_ASSERT(r3.intersects(r4), "both intersection and collision counts");

  // Rectangle inside another rectangle
  Rect r5(10, 10, 50, 50);
  Rect r6(20, 20, 10, 10);
  CX_ASSERT(r5.intersects(r6),
            "Test 3: Rectangle inside another rectangle should intersect");

  // Rectangles with intersecting area
  Rect r7(10, 10, 50, 50);
  Rect r8(40, 40, 50, 50);
  CX_ASSERT(r7.intersects(r8),
            "Test 4: Overlapping rectangles should intersect");

  // Non-intersecting rectangles
  Rect r9(10, 10, 50, 50);
  Rect r10(100, 100, 50, 50);
  CX_ASSERT(!r9.intersects(r10),
            "Test 5: Non-overlapping rectangles should not intersect");
  // Test 1: Circle inside rectangle
  std::cout << "  Testing circle inside rectangle..." << std::endl;
  Circle c1(5, 5, 1);
  Rect r11(0, 0, 10, 10);
  CX_ASSERT(r11.intersects(c1), "Circle inside rectangle should intersect");

  // Test 2: Circle intersecting rectangle at edge
  std::cout << "  Testing circle intersecting rectangle at edge..."
            << std::endl;
  Circle c2(10, 5, 5);
  Rect r12(0, 0, 10, 10);
  CX_ASSERT(r12.intersects(c2),
            "Circle intersecting rectangle at edge should intersect");

  // Test 3: Circle not intersecting rectangle
  std::cout << "  Testing circle not intersecting rectangle..." << std::endl;
  Circle c3(15, 15, 1);
  Rect r13(0, 0, 10, 10);
  CX_ASSERT(!r13.intersects(c3),
            "Circle not intersecting rectangle should not intersect");

  // Test 4: Circle intersecting rectangle at corner
  std::cout << "  Testing circle intersecting rectangle at corner..."
            << std::endl;
  Circle c4(10, 10, 5);
  Rect r14(0, 0, 10, 10);
  CX_ASSERT(r14.intersects(c4),
            "Circle intersecting rectangle at corner should intersect");


  std::cout << "CIRCLE TESTS:" << std::endl;

  // Circle-Rectangle Intersection Tests
  {
    std::cout << "  Circle-Rectangle Intersection Tests:" << std::endl;

    // Define some rectangles
    Rect r1(0.0, 0.0, 10.0, 10.0);

    // Define some circles
    Circle c1(5.0, 5.0, 5.0);  // Circle inside the Rectangle
    Circle c2(15.0, 5.0, 5.0); // Circle inside the Rectangle
    Circle c3(16.0, 5.0, 5.0); // Circle outside the Rectangle

    // Circle c1 intersects with Rectangle r1
    CX_ASSERT(c1.intersects(r1), "Circle-Rectangle Intersection Test 1 Failed");
    // Circle c2 does  intersect with Rectangle r1
    CX_ASSERT(c2.intersects(r1), "Circle-Rectangle Intersection Test 2 Failed");
    CX_ASSERT(!c3.intersects(r1), "Circle-Rectangle Intersection Test 3 Failed");
  }

  // Circle-Circle Intersection Tests
  {
    std::cout << "  Circle-Circle Intersection Tests:" << std::endl;

    // Define some circles
    Circle c1(0.0, 0.0, 10.0);
    Circle c2(8.0, 0.0, 5.0);   // Circle intersects with c1
    Circle c3(20.0, 0.0, 5.0);  // Circle outside c1

    // Circle c2 intersects with Circle c1
    CX_ASSERT(c2.intersects(c1), "Circle-Circle Intersection Test 1 Failed");
    // Circle c3 does not intersect with Circle c1
    CX_ASSERT(!c3.intersects(c1), "Circle-Circle Intersection Test 2 Failed");
  }

  std::cout << "POINT TESTS:" << std::endl;

  // Point-Rectangle Intersection Tests
  {
    std::cout << "  Point-Rectangle Intersection Tests:" << std::endl;

    // Define some rectangles
    Rect r1(0.0, 0.0, 10.0, 10.0);

    // Define some points
    Point p1(5.0, 5.0);
    Point p2(15.0, 5.0);

    // Point p1 is inside Rectangle r1
    CX_ASSERT(p1.intersects(r1), "Point-Rectangle Intersection Test 1 Failed");
    // Point p2 is outside Rectangle r1
    CX_ASSERT(!p2.intersects(r1), "Point-Rectangle Intersection Test 2 Failed");
  }

  // Point-Circle Intersection Tests
  {
    std::cout << "  Point-Circle Intersection Tests:" << std::endl;

    // Define some circles
    Circle c1(0.0, 0.0, 10.0);

    // Define some points
    Point p1(5.0, 5.0);
    Point p2(15.0, 5.0);

    // Point p1 is inside Circle c1
    CX_ASSERT(p1.intersects(c1), "Point-Circle Intersection Test 1 Failed");
    // Point p2 is outside Circle c1
    CX_ASSERT(!p2.intersects(c1), "Point-Circle Intersection Test 2 Failed");
  }
}
#endif
#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_GEOMETRY_H_
