#include "ReluLibxsmm.h"
#include <libxsmm.h>

at::Tensor mini_dnn::backend::ReluLibxsmm::forward( at::Tensor i_x ) {
  // get involved sizes
  l_sizes_x = i_x.sizes();
  l_m = l_sizes_x[0];
  l_n = l_sizes_x[1];

  libxsmm_meltw_unary_shape l_unary_shape = libxsmm_create_meltw_unary_shape( l_m,
                                                                              l_n,
                                                                              l_m,
                                                                              l_m,
                                                                              LIBXSMM_DATATYPE_F32,
                                                                              LIBXSMM_DATATYPE_F32,
                                                                              LIBXSMM_DATATYPE_F32);
  // generate kernel
  libxsmm_meltwfunction_unary l_relu = libxsmm_dispatch_meltw_unary_v2( LIBXSMM_MELTW_TYPE_UNARY_RELU,
                                    l_unary_shape,
                                    LIBXSMM_MELTW_FLAG_UNARY_NONE );
  // output tensor
  at::Tensor l_y = at::zeros( {l_m, l_n} );

  libxsmm_meltw_unary_param l_param;
  l_param.in.primary = i_x;
  l_param.out.primary = l_y;

  l_relu( &l_param );

  return l_y;
}