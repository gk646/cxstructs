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
#define F
#ifndef CXSTRUCTS_SRC_MACHINELEARNING_K_NN_H_
#define CXSTRUCTS_SRC_MACHINELEARNING_K_NN_H_

#include <vector>
#include "../cxconfig.h"
#include "../cxstructs/Geometry.h"
#include "../cxstructs/HashMap.h"
#include "../cxstructs/QuadTree.h"
#include "../cxstructs/row.h"

/**
 * <h2>k-Nearest Neighbour</h2>
 *
 * is a simple instance-based machine learning algorithm that tries to classify new data points by comparing
 * it to existing ones. Specifically to k number of closest points, hence the name.<p>
 *
 */
namespace cxml {
using namespace cxutil;
using namespace cxstructs;

enum class DISTANCE_FUNCTION_2D { EUCLIDEAN, MANHATTAN };
enum class DISTANCE_FUNCTION_XD { EUCLIDEAN, MANHATTAN, CHEBYSHEV, COSINE };
enum class CLASSIFICATION_TYPE {
  ABSOLUTE_CATEGORY,   // Classification based on absolute count of most occurring category
  DISTANCE_WEIGHTED,   // Classification based on distance weighted with type
  DISTANCE_AND_WEIGHT  // Classification based on distance and weight weighted with type
};

/**
 * @tparam C the categories
 */
template <typename C>  // category
struct DataPoint_ {
  using Category = C;
  virtual float x() const = 0;
  virtual float y() const = 0;
  virtual float getWeight() const = 0;
  virtual C getCategory() = 0;
};

template <typename DP_>
class kNN_2D {
  using Category = typename DP_::Category;
  D_func dist_func;
  QuadTree<DP_> space;

  uint_32_cx n_points;
  DP_* data_ptr;

 public:
  kNN_2D(std::vector<DP_>& data, DISTANCE_FUNCTION_2D distance_function, Rect bounds = {})
      : space({}), data_ptr(data.data()), n_points(data.size()) {
    if (distance_function == DISTANCE_FUNCTION_2D::EUCLIDEAN) {
      dist_func = cxutil::euclidean;
    } else {
      dist_func = cxutil::manhattan;
    }
    if (bounds.width() == 0 && bounds.height() == 0) {
      float max_x = std::numeric_limits<float>::min();
      float max_y = std::numeric_limits<float>::min();
      float min_x = std::numeric_limits<float>::max();
      float min_y = std::numeric_limits<float>::max();
      for (auto& dp : data) {
        if (dp.x() > max_x) {
          max_x = dp.x();
        }
        if (dp.x() < min_x) {
          min_x = dp.x();
        }
        if (dp.y() > max_y) {
          max_y = dp.y();
        }
        if (dp.y() < min_y) {
          min_y = dp.y();
        }
      }
      space.set_bounds({min_x, min_y, max_x - min_x, max_y - min_y});
    }
    for (auto& dp : data) {
      space.insert(std::move(dp));
    }
  }
  inline Category classify_new(float x, float y, int k) {
    vec<int> catg_values(128,0);
    vec<DP_*> k_closest;
    Rect search_space = {x, y, 1, 1};

    while (k_closest.size() < k) {  //expanding till at least k size
      k_closest = space.get_subrect(search_space);
      search_space.x()--;
      search_space.y()--;
      search_space.height() += 2;
      search_space.width() += 2;
    }

    if (k_closest.size() > k) {
      std::cout<< "too  big" << std::endl;
      //sorting with custom comparator, ascending to distance
      k_closest.sort([this, x, y](DP_* f, DP_* s) {
        float dist1 = dist_func(x, y, f->x(), f->y());
        float dist2 = dist_func(x, y, s->x(), s->y());
        return dist1 < dist2;
      });

      int diff = k_closest.size() - k;

      for (uint_fast32_t i = 0; i < diff; i++) {  // erasing last elements
        k_closest.pop_back();
      }
    }

    for (auto& ptr : k_closest) {
      catg_values[ptr->getCategory()]++;
    }

    int most = -1;
    int index = 0;
    for (uint_fast32_t i = 0; i < catg_values.size(); i++) {
      if(catg_values[i] > most){
        most = catg_values[i];
        index = i;
      }
    }
    if (most == -1) {
      return Category::NONE;
    }

    return Category(index);
  }

#ifndef CX_DELETE_TESTS
  static void TEST() {
    enum Category { A, B, C, NONE };
    struct DataPoint : public DataPoint_<Category> {
      Category category;
      float x_;
      float y_;
      DataPoint(float x, float y, Category category) : x_(x), y_(y), category(category) {}
      float x() const final { return x_; }
      float y() const final { return y_; }
      Category getCategory() final { return category; }
      float getWeight() const override { return 0; }
    };

    std::vector<DataPoint> data{};
    data.emplace_back(1, 2, Category::A);
    data.emplace_back(2, 3, Category::A);
    data.emplace_back(3, 4, Category::A);
    data.emplace_back(4, 5, Category::B);
    data.emplace_back(5, 6, Category::B);
    data.emplace_back(6, 7, Category::B);
    data.emplace_back(7, 8, Category::C);
    data.emplace_back(8, 9, Category::C);
    data.emplace_back(9, 10, Category::C);
    kNN_2D<DataPoint> knn(data, DISTANCE_FUNCTION_2D::EUCLIDEAN);
    auto cat_1 = knn.classify_new(0, 0, 4);
    CX_ASSERT((int)cat_1 ==0);
  }
#endif
};
class kNN_XD {};

}  // namespace cxml
#endif  //CXSTRUCTS_SRC_MACHINELEARNING_K_NN_H_
