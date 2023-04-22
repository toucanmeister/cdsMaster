#include <iostream>
#include <stdlib.h>
#include <mpi.h>
#include <chrono>

#define N 16384
#define MASTER 0
#define MEASUREMENTS 1000000

// This program assumes that N is divisible by the number of processes
// And for the tree version it assumes that p = 2^d

void compute_data(double* x, double* y, int block_size, int block_start) {
    for (int i=0; i < block_size; i++) {
        int idx = block_start + i;
        x[idx] = idx+1;
        y[idx] = N - idx;
    }
    
}

double subsum(double* x, double* y, int block_size, int block_start) {
    double t = 0;
    for (int i=0; i < block_size; i++) {
        int idx = block_start + i;
        t += x[idx] * y[idx];
    }
    return t;
}

void gather(double* t, int rank, int size) {
    double tmp;
    if (rank == MASTER) {
        for (int p=0; p < size; p++) {
            if (p == MASTER) continue;
            MPI_Recv(&tmp, 1, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            *t += tmp;
        }
    } else {
        MPI_Send(t, 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
    }
}

void tree_gather(double* t, int rank, int log2_size) {
    for (int i=0; i < log2_size; i++) {
        int m = 1 << i;  // m = 2^i
        if (rank & m) {
            MPI_Send(t, 1, MPI_DOUBLE, (rank & ~m), 0, MPI_COMM_WORLD);
        } else {
            double tmp;
            MPI_Recv(&tmp, 1, MPI_DOUBLE, (rank | m), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            *t += tmp;
        }
    }
}

void fat_tree_gather(double* t, int rank, int log2_size) {
    for (int i=0; i < log2_size; i++) {
        int m = 1 << i; // m = 2^i
        if (rank & m) {
            double tmp;
            MPI_Recv(&tmp, 1, MPI_DOUBLE, (rank & ~m), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(t, 1, MPI_DOUBLE, (rank & ~m), 0, MPI_COMM_WORLD);
            *t += tmp;
        } else {
            double tmp;
            MPI_Send(t, 1, MPI_DOUBLE, (rank | m), 0, MPI_COMM_WORLD);
            MPI_Recv(&tmp, 1, MPI_DOUBLE, (rank | m), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            *t += tmp;
        }
    }
}

int my_log2(int n) { // computes d for n = 2^d
    int l = 0;
    while (n >>= 1) { // compute position of highest 1-bit
        l++;
    }
    return l;
}

double linear_dot(double* x, double* y, int block_size, int block_start, int rank, int size) {
    double t = subsum(x, y, block_size, block_start);
    gather(&t, rank, size);
    return t;
}

double tree_dot(double* x, double* y, int block_size, int block_start, int rank, int size) {
    double t = subsum(x, y, block_size, block_start);
    int log2_size = my_log2(size);
    tree_gather(&t, rank, log2_size);
    return t;
}

double fat_tree_dot(double* x, double* y, int block_size, int block_start, int rank, int size) {
    double t = subsum(x, y, block_size, block_start);
    int log2_size = my_log2(size);
    fat_tree_gather(&t, rank, log2_size);
    return t;
}


int main(int argc,char *argv[]){
    int rank, size;
    MPI_Init(&argc,&argv); // MPI starten
    MPI_Comm_size(MPI_COMM_WORLD,&size); // Anzahl der Prozesse ermitteln
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // Nummer des aktuellen Prozesse

    double* x = (double*) malloc(sizeof(double)*N);
    double* y = (double*) malloc(sizeof(double)*N);
    int block_size = N/size; // number of elements of one vector each process has
    int block_start = rank * block_size;
    compute_data(x,y, block_size, block_start);

    // a)
    double mean = 0;
    for (int measurement=0; measurement < MEASUREMENTS; measurement++) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto begin = std::chrono::high_resolution_clock::now();
        double solution_linear = linear_dot(x, y, block_size, block_start, rank, size);
        MPI_Barrier(MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        mean += ((double) elapsed.count()) / MEASUREMENTS;
        if (measurement == 0 && rank == MASTER) std::cout << "Solution linear: " << solution_linear << std::endl;
    }
    if (rank == MASTER) {
        std::cout << "Linear took   " << mean << "ns on average" << std::endl;
    }

    // b)
    mean = 0;
    for (int measurement=0; measurement < MEASUREMENTS; measurement++) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto begin = std::chrono::high_resolution_clock::now();
        double solution_tree = tree_dot(x, y, block_size, block_start, rank, size);
        MPI_Barrier(MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        mean += ((double) elapsed.count()) / MEASUREMENTS;
        if (measurement == 0 && rank == MASTER) std::cout << "Solution tree: " << solution_tree << std::endl;
    }
    if (rank == MASTER) {
        std::cout << "Tree took     " << mean << "ns on average" << std::endl;
    }

    // c)
    mean = 0;
    for (int measurement=0; measurement < MEASUREMENTS; measurement++) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto begin = std::chrono::high_resolution_clock::now();
        double solution_fat_tree = fat_tree_dot(x, y, block_size, block_start, rank, size);
        MPI_Barrier(MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        mean += ((double) elapsed.count()) / MEASUREMENTS;
        if (measurement == 0) std::cout << "Solution fat tree: " << solution_fat_tree << " on process " << rank << std::endl;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == MASTER) {
        std::cout << "Fat tree took " << mean << "ns on average" << std::endl;
    }
    MPI_Finalize(); // MPI beenden
}