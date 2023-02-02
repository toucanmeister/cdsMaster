#include <iostream>
#include <chrono>
using namespace std;

// Exakte Lösung (durch Ausprobieren herausgefunden)
double exact(double x, double y) {
    return (x*x * y*y - x*x * y - x * y*y + x * y) / 2;
}

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

double** jacobi(double h, double eps) {
    int N = 1/h;
    // initialize solution, including boundary
    double** x_old = allocate_array(N+2, N+2);
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            x_old[i][j] = 0;
        }
    }

    // initialize right side
    double** b = allocate_array(N+2, N+2);
    double i_coord = 0;
    double j_coord = 0;
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            b[i][j] = f(i_coord, j_coord);
            j_coord += h;
        }
        i_coord += h;
    }

    double** x = allocate_array(N+2, N+2);
    bool done = false;
    while(!done) {
        // actual update of x
        #pragma omp parallel for
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                double s = -x_old[i-1][j] - x_old[i+1][j] - x_old[i][j-1] - x_old[i][j+1];
                x[i][j] = (b[i][j] - s) / 4;
            }
        }
        // check for convergence
        double largestDifference = 0;
        #pragma omp parallel for
        for (int i = 0; i <= N+1; i++) {
			for (int j = 0; j <= N+1; j++) {
				if (largestDifference < abs(x[i][j] - x_old[i][j])) {
					largestDifference = abs(x[i][j] - x_old[i][j]);
				}
			}
		}
        if (largestDifference < eps) {
            done = true;
        }
        
        // overwrite x_old with x
        #pragma omp parallel for
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
    double** x_old = allocate_array(N+2, N+2);
    double** x = allocate_array(N+2, N+2);
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            x_old[i][j] = 0;
            x[i][j] = 0;
        }
    }

    // initialize right side
    double** b = allocate_array(N+2, N+2);
    double i_coord = 0;
    double j_coord = 0;
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            b[i][j] = f(i_coord, j_coord);
            j_coord += h;
        }
        i_coord += h;
    }

    bool done = false;
    while(!done) {
        // actual update of x
        for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
                double s = -x[i-1][j] - x[i+1][j] - x[i][j-1] - x[i][j+1];
                x[i][j] = (b[i][j] - s) / 4;
            }
        }
        // check for convergence
        double largestDifference = 0;
        for (int i = 0; i <= N+1; i++) {
			for (int j = 0; j <= N+1; j++) {
				if (largestDifference < abs(x[i][j] - x_old[i][j])) {
					largestDifference = abs(x[i][j] - x_old[i][j]);
				}
			}
		}
        if (largestDifference < eps) {
            done = true;
        }
        
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
    double eps = 0.01;
    auto start = chrono::high_resolution_clock::now();
    double** x = jacobi(h, eps);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Time taken: " << duration.count() << "ms" << endl;
    double** y = gauss_seidel(h, eps);
    deallocate_array(x, N+2);
}

// a) und b)
// Entweder sind meine Implementationen nicht ganz richtig, oder meine exakte Lösung stimmt nicht.
// Ich bekomme jedenfalls nicht das selbe heraus.

// c) (Zeiten jeweils als Durchschnitt von drei Ausführungen)
// Serieller Jacobi:  1928ms
// Paralleler Jacobi:  631ms
// Bei einer CPU mit 6 Kernen

// d) Das Gauß-Seidel-Verfahren nutzt in jeder Iteration die Werte dieser Iteration. Durch diese Abhängigkeit innerhalb von Iterationen kann es nicht ohne weiteres parallelisiert werden.