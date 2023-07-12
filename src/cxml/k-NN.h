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
  virtual  C getCategory() = 0;
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
    vec<float> values;
    std::cout<< space.get_bounds() << std::endl;
    for(auto ptr : space.get_subrect(space.get_bounds())){
      std::cout<< ptr->x() << std::endl;
    }

    return Category::A;
  }

#ifndef CX_DELETE_TESTS
  static void TEST() {
    enum Category { A, B, C };
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
    data.emplace_back(1, 2, A);
    data.emplace_back(10, 22, B);
    data.emplace_back(5, 22, C);
    data.emplace_back(5, -3, A);
    kNN_2D<DataPoint> knn(data, DISTANCE_FUNCTION_2D::EUCLIDEAN);
    auto cat_1 = knn.classify_new(2, 2, 10);
    std::cout << cat_1 << std::endl;
  }
#endif
};
class kNN_XD {};

}  // namespace cxml
#endif  //CXSTRUCTS_SRC_MACHINELEARNING_K_NN_H_
