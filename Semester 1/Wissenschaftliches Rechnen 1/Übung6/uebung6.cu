#include <iostream>
#include <chrono>
using namespace std;

double f(double x, double y) {
    return x*(1-x) + y*(1-y);
}

double* allocate_array(int N, int M) {
    return (double*) malloc(sizeof(double)*N*M);
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
    free(x_old);
    free(b);
    return x;
}

__global__  void calculate_x(double* x_old, double* x, double* b, int N, double h) {
	// Find indices in grid
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	// Update x
	x[i*(N+2)+j] = 0.25*(h*h*b[i*(N+2)+j] + x_old[(i-1)*(N+2)+j] + x_old[(i+1)*(N+2)+j] + x_old[i*(N+2)+j-1] + x_old[i*(N+2)+j+1]);
}


double* jacobi_cuda(int N) {
  	// Allocate variables on host 
	double h = 1.0 / (double) N;
	double *b = initialize_b(N, h);
    double *x = initialize_x(N, h);
	
	// Allocate variables on device
	size_t arrsize = sizeof(double)*(N+2)*(N+2);
	double *d_b, *d_x_old, *d_x;
	cudaMalloc((void **) &d_b, arrsize);
	cudaMalloc((void **) &d_x_old, arrsize);
	cudaMalloc((void **) &d_x, arrsize);
	
	// Copy data to device
	cudaMemcpy(d_b, b, arrsize, cudaMemcpyHostToDevice);
	cudaMemcpy(d_x_old, x, arrsize, cudaMemcpyHostToDevice);

	// We have 1024*1024 values to calculate in one iteration
	// 256 threads is a fairly safe number, thus the number of blocks required is 4096
	// We access these 2-dimensionally
	dim3 blocks(64,64,1);
	dim3 threads(16,16,1);
	
    for (int iteration=0; iteration < 2000; iteration++) {
		// Calculate new x values
        calculate_x<<<blocks,threads>>>(d_b, d_x_old, d_x, N, h);
        cudaDeviceSynchronize();
		
		// Overwrite x_old with x once all kernels are finished
		double *tmp = d_x_old;
		d_x_old = d_x;
		d_x = tmp;
    }
	
	// Copy data to host
	cudaMemcpy(x, d_x, arrsize, cudaMemcpyDeviceToHost);
	
    free(b);
	cudaFree(d_x_old);
	cudaFree(d_b);
	cudaFree(d_x);
    return x;
}

int main() {
    int N = 1024;
    
    auto start = chrono::high_resolution_clock::now();
    double* x = jacobi(N);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done with Jacobi!" << endl;
    cout << "Time taken: " << duration.count() << "ms" << endl;

    start = chrono::high_resolution_clock::now();
    double* y = jacobi_cuda(N);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done with Jacobi-CUDA!" << endl;
    cout << "Time taken: " << duration.count() << "ms" << endl;

    free(x);
    free(y);
}
/*
Ausgabe:
Done with Jacobi! Time taken: 9830ms
Done with Jacobi-CUDA! Time taken: 9306ms
*/