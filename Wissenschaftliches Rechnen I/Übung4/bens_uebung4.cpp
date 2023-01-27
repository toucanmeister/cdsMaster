#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <omp.h>
#include <mutex>

using namespace std;

// Gitter mit Größe dim*dim
double** allocGrid(int dim){
    double **grid = new double*[dim];
    double *a = new double[dim*dim];
    for(int i=0; i<dim; i++){
        grid[i] = &a[dim*i];
    }
    return grid;
}

void freeGrid(double** grid){
    free(grid[0]);
    free(grid);
}

void print_grid(double** grid, int dim_t, int dim_x){
    for(int i = 0; i < dim_t; i++){
        for (int j = 0; j < dim_x; j++){
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

void init_rand(double** grid, int dim){
    for (int i = 0; i < dim; i++){
        grid[0][i] = 0.0;
        grid[dim-1][i] = 0.0;
        grid[i][0] = 0.0;
        grid[i][dim-1] = 0.0;
    }
}

void init_b(double** grid, int dim, double h){
    for (int i = 1; i < dim-1; i++){
        for (int j = 1; j < dim-1; j++){
            grid[i][j] = j*h*(1-j*h) + i*h*(1-i*h); 
        }
    }
}

void init_u(double** grid, int dim){
    for (int i = 1; i < dim-1; i++){
        for (int j = 1; j < dim-1; j++){
            grid[i][j] = 1.0; 
        }
    }
}


double jacobi_step(double** u_new, double** u_old, double** b, double h, int dim){
    double delta = 0.0;
    double error = 0.0;
    for (int i = 1; i < dim-1; i++){
        for (int j = 1; j < dim-1; j++){
            u_new[i][j] = 0.25*(h*h*b[i][j] + u_old[i+1][j] + u_old[i-1][j] + u_old[i][j-1] + u_old[i][j+1]);
            delta = fabs(u_new[i][j] - u_old[i][j]);
            if (delta > error){
                error = delta;
            } 
        }
    }
    return error;    
}

double jacobi_step_parallel(double** u_new, double** u_old, double** b, double h, int dim){
    mutex mut;
    double global_error = 0.0;
#pragma omp parallel //num_threads(2)
{
    double delta = 0.0;
    double error = 0.0;
    auto thread_id = omp_get_thread_num();
    auto thread_num = omp_get_num_threads();
    // cout << "Threads: " << thread_num << endl;
    for (int i = 1; i < dim-1; i++){
        for (int j = 1+thread_id; j < dim-1; j+=thread_num){
            u_new[i][j] = 0.25*(h*h*b[i][j] + u_old[i+1][j] + u_old[i-1][j] + u_old[i][j-1] + u_old[i][j+1]);
            delta = fabs(u_new[i][j] - u_old[i][j]);
            if (delta > error){
                error = delta;
            } 
        }
    }
    mut.lock();
    if(error > global_error)    
        global_error = error;
    mut.unlock();    
}

    return global_error;    
}

double gauss_seidel_step(double** u, double** b, double h, int dim){
    double buffer = 0.0;
    double delta = 0.0;
    double error = 0.0;
    for (int i = 1; i < dim-1; i++){
        for (int j = 1; j < dim-1; j++){
            buffer = u[i][j];
            u[i][j] = 0.25*(h*h*b[i][j] + u[i+1][j] + u[i-1][j] + u[i][j-1] + u[i][j+1]);
            delta = fabs(u[i][j] - buffer);
            if (delta > error){
                error = delta;
            } 
        }
    }
    return error;
}

void resetGrid(double** grid, int dim){
    for (int i = 1; i < dim-1; i++){
        for (int j = 1; j < dim-1; j++){
            grid[i][j] = 1;
        }
    }    
}

double exact(double x, double y) {
    return (x*x * y*y - x*x * y - x * y*y + x * y) / 2;
}

double max_error(double** arr, int N, double h) {
    double largestDifference = 0;
    double i_coord = 0;
    double j_coord = 0;
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            double diff = abs(arr[i][j] - exact(i_coord, j_coord));
            if (largestDifference < diff) {
				largestDifference = diff;
			}
			j_coord += h;
        }
        i_coord += h;
    }
    return largestDifference;
}

int main(int argc, char* argv[]) {
    int dim = 100;
    double precision = 1e-5;
    if(argc > 1){
        dim = atoi(argv[1]);        
    }
    if(argc > 2){
        sscanf(argv[2], "%lf", &precision);      
    }
// Initialisieren:

    double h = 1.0/(dim-1);
    double **u_new = allocGrid(dim);
    double **u_old = allocGrid(dim);
    double **b = allocGrid(dim);
    
    init_rand(u_new, dim);
    init_rand(u_old, dim);
    init_rand(b, dim);

    init_b(b, dim, h);
    init_u(u_new, dim);
    init_u(u_old, dim);

// Aufgabe a) Jacobi-Verfahren
    double error = 1.0;
    int iterations = 0;
    double **swap;
    double start_time, end_time;
    start_time = omp_get_wtime();
    while (error > precision){
        iterations++;
        error = jacobi_step(u_new, u_old, b, h, dim);
        swap = u_old;
        u_old = u_new;
        u_new = swap;
    }
    end_time = omp_get_wtime();
    double time_jacobi = end_time - start_time; 
    cout << endl << endl << "Result Jacobi after " << iterations <<  " steps: " << endl << endl;
    //print_grid(u_old, dim, dim);
    cout << "Largest Error Jacobi: " << max_error(u_old, dim-2, h) << endl;

    
// Aufgabe b) Gauß-Seidel-Verfahren
    
    resetGrid(u_old, dim);
    resetGrid(u_new, dim);
    iterations = 0;
    error = 1.0;

    start_time = omp_get_wtime();
    while (error > precision){
        iterations++;
        error = gauss_seidel_step(u_new, b, h, dim);
    }
    end_time = omp_get_wtime();
    double gausTime = end_time - start_time;

    cout << endl << endl << "Result Gauß-Seidel after " << iterations <<  " steps: " << endl << endl;
    //print_grid(u_new, dim, dim);


//Aufgabe c) paralleles Jacobi-Verfahren mit OpenMP

    resetGrid(u_old, dim);
    resetGrid(u_new, dim);
    iterations = 0;
    error = 1.0;
    int numThreads;
    #pragma omp parallel
    {
        numThreads = omp_get_num_threads();
    }
    start_time = omp_get_wtime(); 
    while (error > precision){
        iterations++;
        error = jacobi_step_parallel(u_new, u_old, b, h, dim);
        swap = u_old;
        u_old = u_new;
        u_new = swap;
    }
    end_time = omp_get_wtime();
    double time_paraJacobi = end_time - start_time;

// Ausgabe:
    cout << endl << endl << "Result parallel Jacobi after " << iterations <<  " steps: " << endl << endl;
    //print_grid(u_old, dim, dim);
    cout << "Grid size: " << dim << "x" << dim << ", precision: " << precision << endl;
    cout << "Time Jacobi: " << time_jacobi << "s" << endl;
    cout << "Time parallel Jacobi with " << numThreads << " threads: " << time_paraJacobi << "s" << endl;
    cout << "Time Gauß-Seidel: " << gausTime << "s" << endl;

    freeGrid(u_new);
    freeGrid(u_old);
    freeGrid(b);
    return 0;
}