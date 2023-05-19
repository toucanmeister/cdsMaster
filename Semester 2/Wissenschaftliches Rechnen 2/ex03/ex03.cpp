#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cmath>

#define N 8 // N+2 should be divisible by NUM_BLOCKS_SQRT
#define NUM_BLOCKS_SQRT 2  
#define NUM_BLOCKS NUM_BLOCKS_SQRT*NUM_BLOCKS_SQRT // Number of processes has to be NUM_BLOCKS
#define BLOCK_SIDE (N+2) / NUM_BLOCKS_SQRT
#define NUM_ITER 10000
#define MASTER 0

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
void initialize_space(float* u, int side_size) {
    for (int i=0; i < side_size; i++) {
        for (int j=0; j < side_size; j++) {
            if (i == 0 || j == 0 || i == (side_size-1) || j == (side_size-1)) {
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

int block_row(int rank) {
    return rank / NUM_BLOCKS_SQRT; 
}

int block_col(int rank) {
    return rank % NUM_BLOCKS_SQRT;
}

int block_start(int rank) { // number of the element of u at which this processor's block starts
    return block_row(rank)*BLOCK_SIDE*(N+2) + block_col(rank)*BLOCK_SIDE;
}

void read_block(float* in, float* out, int rank) { // reads the block of the processor with this rank into out
    for (int i=0; i < BLOCK_SIDE; i++) {
        for (int j=0; j < BLOCK_SIDE; j++) {
            int i_coord;
            if (i == 0) {
                i_coord = 0;
            } else { 
                i_coord = (i-1)*(N+2) + (N+2 - block_col(rank)*BLOCK_SIDE);
            }
            out[i*(BLOCK_SIDE) + j] = in[block_start(rank) + i_coord + j];
        }
    }
}

void distribute_data(float* local_u_old, float* local_u_new, float* local_b, int rank) {
    if (rank == MASTER) {
        float* u = (float*) std::malloc(sizeof(float)*(N+2)*(N+2)); // create unit square with resolution of N*N points plus a border
        initialize_space(u);
        float* b = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
        initialize_right_side(b);

        float* tmp_u = (float*) std::malloc(sizeof(float)*BLOCK_SIDE*BLOCK_SIDE);
        float* tmp_b = (float*) std::malloc(sizeof(float)*BLOCK_SIDE*BLOCK_SIDE);
        for (int p=0; p < NUM_BLOCKS; p++) {
            if (p == MASTER) {
                read_block(u, local_u_old, p); // Master gets his own data
                read_block(u, local_u_new, p);
                read_block(b, local_b, p);
            } else {
                read_block(u, tmp_u); 
                read_block(b, tmp_b);
                MPI_Send(tmp_u, BLOCK_SIDE*BLOCK_SIDE, MPI_FLOAT, p, 0, MPI_COMM_WORLD); // Master sends block data to other processes
                MPI_Send(tmp_b, BLOCK_SIDE*BLOCK_SIDE, MPI_FLOAT, p, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(local_u_old, BLOCK_SIDE*BLOCK_SIDE, MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Processes receive block data from master
        MPI_Recv(local_b, BLOCK_SIDE*BLOCK_SIDE, MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        memcpy(local_u_new, local_u_old, sizeof(float)*BLOCK_SIDE*BLOCK_SIDE); 
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc,&argv); // MPI starten
    MPI_Comm_size(MPI_COMM_WORLD,&size); // Anzahl der Prozesse ermitteln
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // Nummer des aktuellen Prozesses

    float* local_u_old = (float*) std::malloc(sizeof(float) * BLOCK_SIDE*BLOCK_SIDE);
    float* local_u_new = (float*) std::malloc(sizeof(float) * BLOCK_SIDE*BLOCK_SIDE);
    float * local_b = (float*) std::malloc(sizeof(float) * BLOCK_SIDE*BLOCK_SIDE);

    distribute_data(local_u_old, local_u_new, local_b, rank, size);

    for (int i=0; i < BLOCK_SIDE; i++) {
        for (int j=0; j < BLOCK_SIDE; j++) {
            std::cout << local_u_new[i*(BLOCK_SIDE) + j] << std::endl;
        }
        std::cout << endl;
    }
    /*
    for (int iteration=0; iteration < NUM_ITER; iteration++) {
        jacobi_step(u_new, u_old, b);

        // this iterations u_new becomes next iteration's u_old
        float* tmp = u_old;
        u_old = u_new;
        u_new = tmp;
    }

    float* u_exact = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    fill_in_exact_solution(u_exact);
    
    //print_array(u_new);
    //print_array(u_exact);
    std::cout << mean_error(u_old, u_exact) << std::endl;
    */

    free(u_new);
    free(u_old);
    free(u_exact);
    free(b);
    return 0;
}