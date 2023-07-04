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
#ifndef CXSTRUCTS_SRC_DATASTRUCTURES_QUADTREE_H_
#define CXSTRUCTS_SRC_DATASTRUCTURES_QUADTREE_H_

#include "../cxconfig.h"
#include "Geometry.h"
#include "vec.h"

namespace cxstructs {
class QuadTree {
  uint_16_cx max_depth_;
  uint_16_cx max_points_;
  Rect bounds_;
  vec<Point, false> vec_;

  QuadTree* top_left_;
  QuadTree* top_right_;
  QuadTree* bottom_left_;
  QuadTree* bottom_right_;

  inline void split() {
    top_left_ = new QuadTree(
        {bounds_.x(), bounds_.y(), bounds_.width() / 2, bounds_.height() / 2},
        max_depth_ - 1, max_points_);
    top_right_ = new QuadTree({bounds_.x() + bounds_.width() / 2, bounds_.y(),
                               bounds_.width() / 2, bounds_.height() / 2},
                              max_depth_ - 1, max_points_);
    bottom_left_ =
        new QuadTree({bounds_.x(), bounds_.y() + bounds_.height() / 2,
                      bounds_.width() / 2, bounds_.height() / 2},
                     max_depth_ - 1, max_points_);
    bottom_right_ = new QuadTree(
        {bounds_.x() + bounds_.width() / 2, bounds_.y() + bounds_.height() / 2,
         bounds_.width() / 2, bounds_.height() / 2},
        max_depth_ - 1, max_points_);
    for (const auto& p : vec_) {
      insert_subtrees(p);
    }
    vec_.clear();
  };
  inline void insert_subtrees(const Point& p) const {
    if (p.x() > bounds_.x() + bounds_.width() / 2) {
      if (p.y() > bounds_.y() + bounds_.height() / 2) {
        bottom_right_->insert(p);
      } else {
        top_right_->insert(p);
      }
    } else {
      if (p.y() > bounds_.y() + bounds_.height() / 2) {
        bottom_left_->insert(p);
      } else {
        top_left_->insert(p);
      }
    }
  }
  inline void get_subtree_size(uint_32_cx& current) const {
    current += vec_.size();
    if (top_right_) {
      top_right_->get_subtree_size(current);
      top_left_->get_subtree_size(current);
      bottom_right_->get_subtree_size(current);
      bottom_left_->get_subtree_size(current);
    }
  }

 public:
  explicit QuadTree(Rect initial_bounds, uint_16_cx max_depth = 10,
                    uint_32_cx max_points = 100)
      : max_depth_(max_depth),
        max_points_(max_points),
        bounds_(initial_bounds),
        top_left_(nullptr),
        top_right_(nullptr),
        bottom_left_(nullptr),
        bottom_right_(nullptr){};
  ~QuadTree() {
    delete top_right_;
    delete top_left_;
    delete bottom_right_;
    delete bottom_left_;
  }
  void insert(const Point& p) {
    if (!bounds_.contains(p)) {
      return;
    }

    if (!top_right_) {
      if (vec_.size() < max_points_) {
        vec_.push_back(p);
        return;
      } else {
        if (max_depth_ > 0) {
          split();
        } else {
          vec_.push_back(p);
          CX_WARNING(
              "|QuadTree.h| Reached max depth | large insertions now will slow "
              "down the tree");
        }
      }
    }

    insert_subtrees(p);
  }
  void clear() {
    delete top_right_;
    delete top_left_;
    delete bottom_right_;
    delete bottom_left_;
    bottom_left_ = nullptr;
    top_right_ = nullptr;
    bottom_left_ = nullptr;
    bottom_right_ = nullptr;
    vec_.clear();
  };
  bool contains();
  void erase();
  /**
   * Actively iterates down the tree summing all subtree sizes<p>
   * <b>This is rather slow </b>
   * @return the max depth of the tree
   */
  [[nodiscard]] inline uint_32_cx size() const {
    uint_32_cx size = vec_.size();
    get_subtree_size(size);
    return size;
  }
  /**
   * Actively iterates down the tree for its max depth<p>
   * <b>This is rather slow </b>
   * @return the max depth of the tree
   */
  [[nodiscard]] inline uint_16_cx depth() const {
    auto tree = this->top_right_;
    uint_16_cx depth = 0;
    while (tree) {
      tree = tree->top_right_;
      depth++;
    }
    return depth;
  }
  class Iterator {};
#ifndef CX_DELETE_TESTS
  static void TEST() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(1, 199);
    std::cout << "TESTING QUAD TREE" << std::endl;

    std::cout << "   Testing insert..." << std::endl;
    QuadTree tree({0, 0, 200, 200});
    for (uint_fast32_t i = 0; i < 1000; i++) {
      tree.insert({distr(gen), distr(gen)});
    }
    CX_ASSERT(tree.depth() == 2);
    CX_ASSERT(tree.size() == 1000);
  }
#endif
};
}  // namespace cxstructs
#endif  //CXSTRUCTS_SRC_DATASTRUCTURES_QUADTREE_H_
