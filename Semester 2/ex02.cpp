#include <cstdlib>
#include <ATen/ATen.h>
#include <iostream>

int main() {
  std::cout << "running the ATen examples" << std::endl;

  float l_data[4*2*3] = {  0.0f,  1.0f,  2.0f, 
                           3.0f,  4.0f,  5.0f,

                           6.0f,  7.0f,  8.0f, 
                           9.0f, 10.0f, 11.0f,
                           
                          12.0f, 13.0f, 14.0f,
                          15.0f, 16.0f, 17.0f,
                          
                          18.0f, 19.0f, 20.0f,
                          21.0f, 22.0f, 23.0f };

  std::cout << "l_data (ptr): " << l_data << std::endl;

  //// STORAGE
  // 2.
  at::Tensor l_tensor = at::from_blob(l_data, {4,2,3});

  // 3.
  std::cout << l_tensor << std::endl;
  std::cout << l_tensor.dtype() << std::endl;
  std::cout << l_tensor.sizes() << std::endl;
  std::cout << l_tensor.strides() << std::endl;
  std::cout << l_tensor.storage_offset() << std::endl;
  std::cout << l_tensor.device() << std::endl;
  std::cout << l_tensor.layout() << std::endl;
  std::cout << l_tensor.is_contiguous() << std::endl;
  std::cout << std::endl;

  // 4.
  std::cout << "Changing tensor using aten:" << std::endl;
  l_tensor[1][1][1] = 100.0f;
  std::cout << l_tensor << std::endl;
  std::cout << "Changing tensor using C pointer:" << std::endl;
  l_data[6+3+1] = 200.0f;
  std::cout << l_tensor << std::endl;

  // 5.
  at::Tensor l_view = l_tensor.select(1,1);
  std::cout << "View:" << std::endl << l_view << std::endl;
  l_view[0][0] = 500.0f;
  std::cout << "Edited view:" << std::endl << l_view << std::endl;
  std::cout << "Also changed the original tensor:" << std::endl << l_tensor << std::endl;

  //6.
  at::Tensor l_cont = l_view.contiguous();
  std::cout << "Contiguous view:" << std::endl << l_cont << std::endl;
  // l_cont is created from l_view by allocating new memory for the data which l_view references 
  // and storing it contiguously. This means it is not a view but actually new memory and also changes the strides.

  //// OPERATIONS
  // 1.
  at::Tensor a = at::rand({16, 4});
  at::Tensor b = at::rand({4, 16});
  std::cout << a << std::endl << b << std::endl;

  // 2.
  std::cout << a.matmul(b) << std::endl;

  // 3.
  at::Tensor t0 = at::rand({16, 4, 2});
  at::Tensor t1 = at::rand({16, 2, 4});

  // 4.
  std::cout << t0.bmm(t1) << std::endl;

  std::cout << "finished running ATen examples" << std::endl;

  return EXIT_SUCCESS;
}

