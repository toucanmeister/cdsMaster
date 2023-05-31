#ifndef MINI_DNN_BACKEND_RELU_LIBXSMM_H
#define MINI_DNN_BACKEND_RELU_LIBXSMM_H

#include "Matmul.hpp"
#include <ATen/ATen.h>

namespace mini_dnn {
  namespace backend {
    class ReluLibxsmm;
  }
}

/**
 * ReLu backend using LIBXSMM.
 **/
class mini_dnn::backend::ReluLibxsmm: public Matmul {
  private:
  public:
    /**
     * Perform the forward pass, i.e., Y = relu(X), elementwise.
     *
     * @param i_x matrix X.
     * @return output of the matmul, i.e., Y.
     **/
    at::Tensor forward( at::Tensor i_x );
};

#endif