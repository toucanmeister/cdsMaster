#include <iostream>
#include <chrono>
using namespace std;

double f(double x, double y) {
    return x*(1-x) + y*(1-y);
}

double** allocate_array(int N, int M) {
    double** arr = new double*[N];
    for (int i=0; i < N; i++) {
        arr[i] = new double[M];
    }
    return arr;
}

void deallocate_array(double** arr, int N) {
    for (int i=0; i < N; i++) {
        delete [] arr[i];
    }
    delete [] arr;
}

double** initialize_x(int N, double h) { // initialize solution, including boundary
    double** x = allocate_array(N+2, N+2);
    for (int i=0; i <= N+1; i++) {
        for (int j=0; j <= N+1; j++) {
            x[i][j] = 0;
        }
    }
    return x;
}

double ** initialize_b(int N, double h) { // initialize right side
    double** b = allocate_array(N+2, N+2);
    double i_coord = 0;
    double j_coord = 0;
    for (int i=0; i <= N+1; i++) {
        for (int j=0; j <= N+1; j++) {
            b[i][j] = f(i_coord, j_coord);
            j_coord += h;
        }
        i_coord += h;
    }
    return b;
}

bool hasConverged(double **x, double **x_old, int N, double eps) {
    double largestDifference = 0;
        for (int i = 0; i <= N+1; i++) {
			for (int j = 0; j <= N+1; j++) {
				if (largestDifference < abs(x[i][j] - x_old[i][j])) {
					largestDifference = abs(x[i][j] - x_old[i][j]);
				}
			}
		}
        if (largestDifference < eps) {
            return true;
        } else {
            return false;
        }
}

double** jacobi(double h, double eps) {
    int N = 1/h;
    double** x_old = initialize_x(N, h);
    double** b = initialize_b(N, h);
    double** x = allocate_array(N+2, N+2);

    bool done = false;
    while(!done) {
        // actual update of x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x[i][j] = 0.25*(h*h*b[i][j] + x_old[i-1][j] + x_old[i+1][j] + x_old[i][j-1] + x_old[i][j+1]);
            }
        }
        done = hasConverged(x, x_old, N, eps);
        // overwrite x_old with x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x_old[i][j] = x[i][j];
            }
        }
    }
    cout << "Done with Jacobi!" << endl;
    deallocate_array(x_old, N+2);
    deallocate_array(b, N+2);
    return x;
}

double** gauss_seidel(double h, double eps) {
    int N = 1/h;
    // initialize solution, including boundary
    double** x_old = initialize_x(N, h);
    double** b = initialize_b(N,h);
    double** x = allocate_array(N+2, N+2);

    bool done = false;
    while(!done) {
        // actual update of x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x[i][j] = 0.25*(h*h*b[i][j] + x[i-1][j] + x[i+1][j] + x[i][j-1] + x[i][j+1]);
            }
        }
        done = hasConverged(x, x_old, N, eps);
        // overwrite x_old with x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                x_old[i][j] = x[i][j];
            }
        }
    }
    cout << "Done with Gauss-Seidel!" << endl;
    deallocate_array(x_old, N+2);
    deallocate_array(b, N+2);
    return x;
}

int main() {
    double h = 0.01;
    int N = 1/h;
    double eps = 0.001;
    
    auto start = chrono::high_resolution_clock::now();
    double** x = jacobi(h, eps);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Time taken: " << duration.count() << "ms" << endl;
    
    start = chrono::high_resolution_clock::now();
    double** y = gauss_seidel(h, eps);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Time taken: " << duration.count() << "ms" << endl;

    deallocate_array(x, N+2);
    deallocate_array(y, N+2);
}
