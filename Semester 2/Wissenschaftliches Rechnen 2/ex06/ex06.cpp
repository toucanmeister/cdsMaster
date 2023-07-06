#include <iostream>
#include <cstdlib>
#include <cmath>

#define N 100
#define OVERLAP 100 // make sure that (N - OVERLAP) is even
#define EPS 1E-9


#define OVERLAP_START (N-OVERLAP)/2
#define OVERLAP_END OVERLAP_START+OVERLAP

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

float gauss_seidel_step(float* u, float* b, int start_x, int end_x) {
    float h = 1.0 / (float) N;
    float max_change = 0;
    for (int i=start_x; i <= end_x; i++) { // perform update on the inner N*N values
        for (int j=1; j <= N; j++) {
            float old = u[idx(i,j)];
            u[idx(i,j)] = 0.25 * (h*h*b[idx(i,j)] + u[idx(i-1,j)] + u[idx(i+1,j)] + u[idx(i,j-1)] + u[idx(i,j+1)]);
            float change = fabs(old - u[idx(i,j)]);
            if (change > max_change) {
                max_change = change;
            }
        }
    }
    return max_change;
}


int main(int argc, char** argv) {
    float* u = (float*) std::malloc(sizeof(float)*(N+2)*(N+2)); // create unit square with resolution of N*N points plus a border
    initialize_space(u);

    float* b = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    initialize_right_side(b);

    int iteration = 0;
    while(true) {
        iteration++;
        float max_change_1, max_change_2;
        for (int i=0; i < 10000; i++) max_change_1 = gauss_seidel_step(u, b, 1, OVERLAP_END);
        for (int i=0; i < 10000; i++) max_change_2 = gauss_seidel_step(u, b, OVERLAP_START, N);
        if (fmax(max_change_1, max_change_2) < EPS) break;
    }

    float* u_exact = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    fill_in_exact_solution(u_exact);
    
    //print_array(u);
    //print_array(u_exact);
    std::cout << "Took " << iteration << " iterations" << std::endl;

    free(u);
    free(u_exact);
    free(b);
    return 0;
}