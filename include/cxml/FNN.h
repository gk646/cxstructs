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
#ifndef CXSTRUCTS_SRC_MACHINELEARNING_FNN_H_
#define CXSTRUCTS_SRC_MACHINELEARNING_FNN_H_

#include <algorithm>
#include <cmath>

#include <iostream>
#include <random>
#include <vector>
#include "../cxconfig.h"

namespace cxhelper {

typedef float (*a_func)(float);
}  // namespace cxhelper

#ifdef CX_MATRIX

#include "../datastructures/mat.h"
#include "../datastructures/row_vec.h"

#pragma message("|FNN.h| Using Matrices for calculations.")

namespace cxhelper {
using namespace cxstructs;
struct Layer {
  mat weights_;
  vec<float> bias_;
  vec<float> w_sums_;
  mat inputs_;
  uint_16_cx in_;
  uint_16_cx out_;
  a_func func;
  a_func d_func;
  float learnR_;

 public:
  Layer()
      : weights_(0, 0),
        bias_(0, 0),
        w_sums_(0, 0),
        in_(0),
        out_(0),
        func(relu),
        d_func(d_relu),
        learnR_(0.5) {}
  Layer(uint_16_cx in, uint_16_cx out, a_func func, float learnR)
      : in_(in), out_(out), func(func), learnR_(learnR), inputs_(1, in) {
    if (func == relu) {
      d_func = d_relu;
    } else if (func == sig) {
      d_func = d_sig;
    } else {
      d_func = [](float x) {
        return static_cast<float>(1.0);
      };
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.3, 0.3);

    weights_ = mat(in, out, [&dis, &gen](int i) { return dis(gen); });
    bias_ = vec<float>(out_, [&dis, &gen](int i) { return dis(gen); });
    w_sums_ = vec<float>(out_, [&dis, &gen](int i) { return dis(gen); });
  }
  Layer(const Layer& other)
      : in_(other.in_),
        out_(other.out_),
        func(other.func),
        learnR_(other.learnR_),
        inputs_(other.inputs_),
        weights_(other.weights_),
        bias_(other.bias_),
        w_sums_(other.w_sums_) {
    d_func = other.d_func;
  }

  // Assignment operator
  Layer& operator=(const Layer& other) {
    if (this != &other) {
      in_ = other.in_;
      out_ = other.out_;
      func = other.func;
      learnR_ = other.learnR_;
      inputs_ = other.inputs_;
      d_func = other.d_func;

      weights_ = other.weights_;
      bias_ = other.bias_;
      w_sums_ = other.w_sums_;
    }
    return *this;
  }
  ~Layer() = default;
  [[nodiscard]] mat forward(mat& in) {
    inputs_ = in;
    mat out(out_, 0);
    float w_sum;
    for (int i = 0; i < out_; i++) {
      w_sum = 0;
      for (int j = 0; j < in_; j++) {
        w_sum += in(0, j) * weights_(j, i);
      }
      w_sums_[i] = w_sum + bias_[i];
      out[i] = func(w_sums_[i]);
    }
    return out;
  }
  [[nodiscard]] std::vector<float> backward(std::vector<float>& error) const {
    std::vector<float> n_error(in_, 0);
    for (int i = 0; i < out_; i++) {
      error[i] *= d_func(w_sums_[i]);
      for (int j = 0; j < in_; j++) {
        n_error[j] += weights_[j * out_ + i] * error[i];
        weights_[j * out_ + i] -= inputs_[j] * error[i] * learnR_;
      }
      bias_[i] -= learnR_ * error[i];
    }
    return n_error;
  }
};
}  // namespace cxhelper

/**
 * <h2>Feedforward Neural Network</h2>
 *
 */
class FNN {};

#else
#pragma message( \
        "|FNN.h| Using FNN.h without '#define CX_MATRIX', calculations are loop based.")
namespace cxhelper {
struct Layer {
  float* weights_;
  float* bias_;
  float* w_sums_;
  std::vector<float> inputs_;
  uint_16_cx in_;
  uint_16_cx out_;
  a_func func;
  a_func d_func;
  float learnR_;

 public:
  Layer()
      : weights_(nullptr),
        bias_(nullptr),
        w_sums_(nullptr),
        in_(0),
        out_(0),
        func(cxutil::relu),
        d_func(cxutil::d_relu),
        learnR_(0.5) {}
  Layer(uint_16_cx in, uint_16_cx out, a_func func, float learnR)
      : in_(in), out_(out), func(func), learnR_(learnR), inputs_(in) {
    if (func == cxutil::relu) {
      d_func = cxutil::d_relu;
    } else if (func == cxutil::sig) {
      d_func = cxutil::d_sig;
    } else {
      d_func = [](float x) {
        return static_cast<float>(1.0);
      };
    }
    weights_ = new float[in_ * out_];
    bias_ = new float[out_];
    w_sums_ = new float[out_];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.3, 0.3);
    for (int i = 0; i < in_ * out_; ++i) {
      weights_[i] = dis(gen);
    }

    for (int i = 0; i < out_; ++i) {
      bias_[i] = dis(gen);
    }
  }
  Layer(const Layer& other)
      : in_(other.in_),
        out_(other.out_),
        func(other.func),
        learnR_(other.learnR_),
        inputs_(other.inputs_) {
    weights_ = new float[in_ * out_];
    bias_ = new float[out_];
    w_sums_ = new float[out_];
    std::copy(other.weights_, other.weights_ + in_ * out_, weights_);
    std::copy(other.bias_, other.bias_ + out_, bias_);
    std::copy(other.w_sums_, other.w_sums_ + out_, w_sums_);
    d_func = other.d_func;
  }

  // Assignment operator
  Layer& operator=(const Layer& other) {
    if (this != &other) {
      delete[] weights_;
      delete[] bias_;
      delete[] w_sums_;

      in_ = other.in_;
      out_ = other.out_;
      func = other.func;
      learnR_ = other.learnR_;
      inputs_ = other.inputs_;
      d_func = other.d_func;

      weights_ = new float[in_ * out_];
      bias_ = new float[out_];
      w_sums_ = new float[out_];
      std::copy(other.weights_, other.weights_ + in_ * out_, weights_);
      std::copy(other.bias_, other.bias_ + out_, bias_);
      std::copy(other.w_sums_, other.w_sums_ + out_, w_sums_);
    }
    return *this;
  }
  ~Layer() {
    delete[] weights_;
    delete[] bias_;
    delete[] w_sums_;
  }
  [[nodiscard]] std::vector<float> forward(std::vector<float>& in) {
    inputs_ = in;
    std::vector<float> out(out_, 0);
    float w_sum;
    for (int i = 0; i < out_; i++) {
      w_sum = 0;
      for (int j = 0; j < in_; j++) {
        w_sum += in[j] * weights_[j * out_ + i];
      }
      w_sums_[i] = w_sum + bias_[i];
      out[i] = func(w_sums_[i]);
    }
    return out;
  }
  [[nodiscard]] std::vector<float> backward(std::vector<float>& error) const {
    std::vector<float> n_error(in_, 0);
    for (int i = 0; i < out_; i++) {
      error[i] *= d_func(w_sums_[i]);
      for (int j = 0; j < in_; j++) {
        n_error[j] += weights_[j * out_ + i] * error[i];
        weights_[j * out_ + i] -= inputs_[j] * error[i] * learnR_;
      }
      bias_[i] -= learnR_ * error[i];
    }
    return n_error;
  }
};
}  // namespace cxhelper
namespace cxml {
using namespace cxhelper;
/**
 * <h2>Feedforward Neural Network</h2>
 * Simple FNN implementation using mean average error and either sigmoid or ReLu
 * The last layer is always linear (better for simple problems and regression tasks)
 */
class FNN {
  Layer* layers_;
  std::vector<uint_16_cx> bounds_;
  uint_16_cx len_;
  float learnR_;

 public:
  explicit FNN(const std::vector<uint_16_cx>& bound, a_func a_func,
               float learnR)
      : learnR_(learnR), len_(bound.size() - 1), bounds_(bound) {
    layers_ = new Layer[len_];
    for (int i = 1; i < len_ + 1; i++) {
      if (i == len_) {
        layers_[i - 1] = Layer(
            bounds_[i - 1], bounds_[i], [](float x) { return x; }, learnR_);
        break;
      }
      layers_[i - 1] = Layer(bounds_[i - 1], bounds_[i], a_func, learnR_);
    }
  }
  ~FNN() { delete[] layers_; }
  std::vector<float> forward(const std::vector<float>& in) {
    std::vector<float> retval = in;
    for (int i = 0; i < len_; i++) {
      retval = layers_[i].forward(retval);
    }
    return retval;
  }

  void train(const std::vector<float>& in, const std::vector<float>& out,
             uint_16_cx n = 10) {
    for (int k = 0; k < n; k++) {
      std::vector<float> retval = forward(in);

      for (int i = 0; i < retval.size(); i++) {
        retval[i] = (retval[i] - out[i]);
      }

      for (int i = len_ - 1; i > -1; i--) {
        retval = layers_[i].backward(retval);
      }
    }
  }
  void train(const std::vector<std::vector<float>>& in,
             const std::vector<std::vector<float>>& out, uint_16_cx n = 10) {

    for (int k = 0; k < n; k++) {

      for (int l = 0; l < in.size(); l++) {
        std::vector<float> retval = forward(in[l]);

        for (int i = 0; i < retval.size(); i++) {
          retval[i] = 2 * (retval[i] - out[l][i]);
        }

        for (int i = len_ - 1; i > -1; i--) {
          retval = layers_[i].backward(retval);
        }
      }
    }
  }

};
}  // namespace cxml

#endif  // CX_MATRIX
#undef CX_MATRIX
#endif  //CXSTRUCTS_SRC_MACHINELEARNING_FNN_H_
