#include <iostream>
#include <cstdlib>
#include <cmath>

#define N 100
#define NUM_ITER 10000

float g(float x, float y) {
    return 0.0;
}

float idx_to_coord(int i) {
    return ((float) i) / ((float) N);
}

float f(float x, float y) {
    return x*(1-x) + y*(1-y);
}

int idx(int i, int j) {
    return i*(N+2) + j;
}

float exact(float x, float y) {
    return (x*x * y*y - x*x * y - x * y*y + x * y) / 2;
}

void fill_in_exact_solution(float* u_exact) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            u_exact[idx(i,j)] = exact(idx_to_coord(i), idx_to_coord(j));
        }
    }
}

void print_array(float* arr) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            std::cout << arr[idx(i,j)] << " ";
        }
        std::cout << std::endl;
    }
}

// initializes u to 0, boundary to g
void initialize_space(float* u) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            if (i == 0 || j == 0 || i == (N+1) || j == (N+1)) {
                u[idx(i,j)] = g(idx_to_coord(i), idx_to_coord(j));
            } else {
                u[idx(i,j)] = 0;
            }
        }
    }
}

// initializes b to the corresponding value of f
void initialize_right_side(float* b) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            b[idx(i,j)] = f(idx_to_coord(i), idx_to_coord(j));
        }
    }
}

float mean_error(float* u, float* u_exact) {
    float e = 0;
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            e += ( fabs(u[idx(i,j)] - u_exact[idx(i,j)]) ) / ((N+2)*(N+2));
        }
    }
    return e;
}

void jacobi_step(float* u_new, float* u_old, float* b) {
    float h = 1.0 / (float) N;
    for (int i=1; i <= N; i++) { // perform update on the inner N*N values
        for (int j=1; j <= N; j++) {
            u_new[idx(i,j)] = 0.25 * (h*h*b[idx(i,j)] + u_old[idx(i-1,j)] + u_old[idx(i+1,j)] + u_old[idx(i,j-1)] + u_old[idx(i,j+1)]);
        }
    }
}

int main(int argc, char** argv) {
    float* u_old = (float*) std::malloc(sizeof(float)*(N+2)*(N+2)); // create unit square with resolution of N*N points plus a border
    initialize_space(u_old);

    float* b = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    initialize_right_side(b);

    float* u_new = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    initialize_space(u_new);

    for (int iteration=0; iteration < NUM_ITER; iteration++) {
        jacobi_step(u_new, u_old, b);

        // this iterations u_new becomes next iteration's u_old
        for (int i=0; i < N+2; i++) {
            for (int j=0; j < N+2; j++) {
                u_old[idx(i,j)] = u_new[idx(i,j)];
            }
        }
    }

    float* u_exact = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    fill_in_exact_solution(u_exact);
    
    //print_array(u_new);
    //print_array(u_exact);
    std::cout << mean_error(u_new, u_exact) << std::endl;

    free(u_new);
    free(u_old);
    free(u_exact);
    free(b);
    return 0;
}