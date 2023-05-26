#include "MatmulAtenBlocked.h"

at::Tensor mini_dnn::backend::MatmulAtenBlocked::forward( at::Tensor i_x,
                                                          at::Tensor i_w ) {
  // get involved sizes
  Matmul::Sizes l_sizes = Matmul::getSizes( i_x,
                                            i_w );

  // prepare data for blocked Aten calls
  at::Tensor l_output = at::zeros( {l_sizes.kb, l_sizes.nb, l_sizes.bk, l_sizes.bn} );

    using namespace at::indexing;

  for( int64_t l_kb = 0; l_kb < l_sizes.kb; l_kb++ ) {
    for( int64_t l_nb = 0; l_nb < l_sizes.nb; l_nb++ ) {
      for( int64_t l_cb = 0; l_cb < l_sizes.cb; l_cb++ ) {
        at::Tensor l_x_block = i_x.index( { l_nb, l_cb, Slice(), Slice() } );
        at::Tensor l_w_block = i_w.index( { l_kb, l_cb, Slice(), Slice() } );
        at::Tensor l_c = at::matmul( l_w_block, l_x_block );
        for (int64_t l_bk = 0; l_bk < l_sizes.bk; l_bk++) {
          for (int64_t l_bn = 0; l_bn < l_sizes.bn; l_bn++) {
            l_output[l_kb][l_nb][l_bk][l_bn] += l_c[l_bk][l_bn];
          }
        }
      }
    }
  }

  return l_output;
}