#include <iostream>
#include <chrono>
#include <immintrin.h>
using namespace std;

double f(double x, double y) {
    return x*(1-x) + y*(1-y);
}

double* allocate_array(int N, int M) {
    return (double*) aligned_alloc(sizeof(double)*4, sizeof(double)*N*M);
}

void deallocate_array(double* arr, int N) {
    free(arr);
}

double* initialize_x(int N, double h) { // initialize solution, including boundary
    double* x = allocate_array(N+2, N+2);
    for (int i=0; i <= N+1; i++) {
        for (int j=0; j <= N+1; j++) {
            x[i*(N+2)+j] = 0;
        }
    }
    return x;
}

double* initialize_b(int N, double h) { // initialize right side
    double* b = allocate_array(N+2, N+2);
    double i_coord = 0;
    double j_coord = 0;
    for (int i=0; i <= N+1; i++) {
        for (int j=0; j <= N+1; j++) {
            b[i*(N+2)+j] = f(i_coord, j_coord);
            j_coord += h;
        }
        i_coord += h;
    }
    return b;
}

double largestDiff(double* x, double* y, int N) {
    double largestDiff = 0;
    for (int i=1; i <= N; i++) {
        for (int j=1; j <= N; j++) {
            double diff = abs(x[i*(N+2)+j] - y[i*(N+2)+j]) > largestDiff;
            if (diff > largestDiff) {
                largestDiff = diff;
            }
        }
    }
    return largestDiff;
}

double* jacobi(int N) {
    double h = 1.0 / (double) N;
    double* x_old = initialize_x(N, h);
    double* b = initialize_b(N, h);
    double* x = allocate_array(N+2, N+2);

    for (int iteration=0; iteration < 2000; iteration++) {
        // update of x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x[i*(N+2)+j] = 0.25*(h*h*b[i*(N+2)+j] + x_old[(i-1)*(N+2)+j] + x_old[(i+1)*(N+2)+j] + x_old[i*(N+2)+j-1] + x_old[i*(N+2)+j+1]);
            }
        }
        // overwrite x_old with x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x_old[i*(N+2)+j] = x[i*(N+2)+j];
            }
        }
    }
    deallocate_array(x_old, N+2);
    deallocate_array(b, N+2);
    return x;
}

inline __m256d loadfour(double *x) {
    return _mm256_set_pd(x[3], x[2], x[1], x[0]);
}

double* jacobi_simd(int N) {
    double h = 1.0 / (double) N;
    double* x_old = initialize_x(N, h);
    double* b = initialize_b(N, h);
    double* x = allocate_array(N+2, N+2);

    for (int iteration=0; iteration < 2000; iteration++) {
        // update of x
        for (int i=1; i < N; i++) {
            for (int j=1; j < N; j+=4) {
                alignas(sizeof(double)*4) __m256d op1, op2, result;
                
                op1 = _mm256_set1_pd(h);
                op2 = _mm256_set1_pd(h);
                result = _mm256_mul_pd(op1, op2); // h*h

                op1 = result;
                op2 = _mm256_loadu_pd(&(b[i*(N+2)+j]));
                result = _mm256_add_pd(op1, op2); // h*h*b[i][j]

                op1 = result;
                op2 = _mm256_loadu_pd(&(x_old[(i-1)*(N+2)+j]));
                result = _mm256_add_pd(op1, op2); // h*h*b[i][j] + x_old[i-1][j]

                op1 = result;
                op2 = _mm256_loadu_pd(&(x_old[(i+1)*(N+2)+j]));
                result = _mm256_add_pd(op1, op2); // h*h*b[i][j] + x_old[i-1][j] + x_old[i+1][j]

                op1 = result;
                op2 = _mm256_loadu_pd(&(x_old[i*(N+2)+j-1]));
                result = _mm256_add_pd(op1, op2); //h*h*b[i][j] + x_old[i-1][j] + x_old[i+1][j] + x_old[i][j-1]

                op1 = result;
                op2 = _mm256_loadu_pd(&(x_old[i*(N+2)+j+1]));
                result = _mm256_add_pd(op1, op2); //h*h*b[i][j] + x_old[i-1][j] + x_old[i+1][j] + x_old[i][j-1] + x_old[i][j+1]

                op1 = result;
                op2 = _mm256_set1_pd(0.25);
                result = _mm256_mul_pd(op1, op2); //0.25 * (h*h*b[i][j] + x_old[i-1][j] + x_old[i+1][j] + x_old[i][j-1] + x_old[i][j+1])

                _mm256_storeu_pd(&(x[i*(N+2)+j]), result);
            }
        }
        // overwrite x_old with x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x_old[i*(N+2)+j] = x[i*(N+2)+j];
            }
        }
    }
    deallocate_array(x_old, N+2);
    deallocate_array(b, N+2);
    return x;
}
// b) 
// Bei Gauss-Seidel hängen die Werte innerhalb einer Iteration voneinander ab.
// Mögliches Vorgehen: Naiv genau so wie bei Jacobi machen. Resultat: Mischlösung zwischen Jacobi und Gauss-Seidel
double* gauss_seidel(int N) {
    double h = 1.0 / (double) N;
    // initialize solution, including boundary
    double* x_old = initialize_x(N, h);
    double* b = initialize_b(N,h);
    double* x = allocate_array(N+2, N+2);

    for (int iteration=0; iteration < 2000; iteration++) {
        // update of x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x[i*(N+2)+j] = 0.25*(h*h*b[i*(N+2)+j] + x[(i-1)*(N+2)+j] + x[(i+1)*(N+2)+j] + x[i*(N+2)+j-1] + x[i*(N+2)+j+1]);
            }
        }
        // overwrite x_old with x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x_old[i*(N+2)+j] = x[i*(N+2)+j];
            }
        }
    }
    deallocate_array(x_old, N+2);
    deallocate_array(b, N+2);
    return x;
}

int main() {
    int N = 1022;
    
    auto start = chrono::high_resolution_clock::now();
    double* x = jacobi(N);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done with Jacobi!" << endl;
    cout << "Time taken: " << duration.count() << "ms" << endl;

    start = chrono::high_resolution_clock::now();
    double* y = jacobi_simd(N);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done with Jacobi-SIMD!" << endl;
    cout << "Time taken: " << duration.count() << "ms" << endl;

    deallocate_array(x, N+2);
    deallocate_array(y, N+2);
}