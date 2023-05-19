#include <iostream>
#include <pybind11/pybind11.h>
#include <torch/extension.h>

void hello() {
    std::cout << "Hello World!" << std::endl;
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("hello",
          &hello,
          "Hello World Function");
}
