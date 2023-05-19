#include <iostream>
#include <pybind11/pybind11.h>
#include <torch/extension.h>

// My own linear algebra functions sadly don't work and
// I've already spent too much time looking for bugs.
// The aten versions work, though!

// Computes i_vec @ i_mat
torch::Tensor mat_vec_mul(torch::Tensor i_vec, torch::Tensor i_mat) {
    float* l_vec_ptr = (float*) i_vec.contiguous().data_ptr();
    float* l_mat_ptr = (float*) i_mat.contiguous().data_ptr();
    int l_vec_size = i_vec.size( 0 );
    int l_out_size = i_mat.size( 1 );
    torch::Tensor l_output = torch::zeros( {l_out_size} );
    float* l_output_ptr = (float*) l_output.data_ptr();
    for ( int i=0; i < l_out_size; i++ ) {
        for ( int j=0; j < l_vec_size; j++ ) {
            l_output_ptr[i] += l_vec_ptr[j] * l_mat_ptr[j*l_out_size + i];
        }
    }
    return l_output;
}

// Computes i_vec @ i_mat + i_b
torch::Tensor mat_vec_mul_add(torch::Tensor i_vec, torch::Tensor i_mat, torch::Tensor i_b) {
    float* l_vec_ptr = (float*) i_vec.contiguous().data_ptr();
    float* l_mat_ptr = (float*) i_mat.contiguous().data_ptr();
    float* l_b_ptr = (float*) i_mat.contiguous().data_ptr();
    int l_vec_size = i_vec.size( 1 );
    int l_out_size = i_mat.size( 1 );
    torch::Tensor l_output = torch::zeros( {1, l_out_size} );
    float* l_output_ptr = (float*) l_output.data_ptr();
    for ( int i=0; i < l_out_size; i++ ) {
        for ( int j=0; j < l_vec_size; j++ ) {
            l_output_ptr[i] += l_vec_ptr[j] * l_mat_ptr[j*l_out_size + i];
        }
        l_output_ptr[i] += l_b_ptr[i];
    }
    return l_output;
}

// Computes i_v1.T @ i_v2
torch::Tensor vec_dot(torch::Tensor i_v1, torch::Tensor i_v2) {
    float* l_v1_ptr = (float*) i_v1.contiguous().data_ptr();
    float* l_v2_ptr = (float*) i_v2.contiguous().data_ptr();
    int l_vec_size = i_v1.size( 0 );
    torch::Tensor l_output = torch::zeros( {1} );
    for ( int i=0; i < l_vec_size; i++ ) {
        l_output[i] += l_v1_ptr[i] * l_v2_ptr[i];
    }
    return l_output;
}

torch::Tensor forward( torch::Tensor i_input,
                       torch::Tensor i_weights ) {
    return i_input.matmul( i_weights.t() );   // aten version

    // return mat_vec_mul( i_input, i_weights.t() );
}

torch::Tensor forward_bias( torch::Tensor i_input,
                            torch::Tensor i_weights,
                            torch::Tensor i_bias ) {
    return i_input.matmul( i_weights.t() ).add( i_bias );   // aten version

    // return mat_vec_mul_add( i_input, i_weights.t(), i_bias );
}

std::vector< torch::Tensor > backward( torch::Tensor i_grad,
                                       torch::Tensor i_input,
                                       torch::Tensor i_weights ) {
    std::vector< torch::Tensor > grads;
    grads.push_back( i_grad.matmul( i_weights ));   // aten version
    grads.push_back( i_grad.t().matmul( i_input ));

    //grads.push_back( mat_vec_mul( i_grad, i_weights) );
    //grads.push_back( vec_dot( i_grad, i_input ));
    return grads;
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("forward",
          &forward,
          "Linear forward");
    m.def("forward_bias",
          &forward_bias,
          "Linear forward with bias");
    m.def("backward",
          &backward,
          "Linear backward");
}