#include <iostream>

int main() {
    double* x = (double*) aligned_alloc(32, sizeof(double)*2*2);
    for (int i=0; i < 2; i++) {
        for (int j=0; j < 2; j++) {
            x[i*2+j] = i+j;
        }
    }
    for (int i=0; i < 2; i++) {
        for (int j=0; j < 2; j++) {
            std::cout << x[i*2+j] << std::endl;
        }
    }
}