#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cmath>

#define N 100 // N+2 should be divisible by NUM_BLOCKS_SQRT
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

float exact(float x, float y) {
    return (x*x * y*y - x*x * y - x * y*y + x * y) / 2;
}

void fill_in_exact_solution(float* u_exact) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            u_exact[i*(N+2) + j] = exact(idx_to_coord(i), idx_to_coord(j));
        }
    }
}

void print_array(float* arr) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            std::cout << arr[i*(N+2) + j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_local_array(float* arr, int rank) {
    for (int i=0; i < BLOCK_SIDE+2; i++) {
        std::cout << rank << " | ";
        for (int j=0; j < BLOCK_SIDE+2; j++) {
            std::cout << arr[i*(BLOCK_SIDE+2) + j] << " ";
        }
        std::cout << std::endl;
    }
}

// initializes u to 0, boundary to g
void initialize_space(float* u) {
    for (int i=0; i < (N+2); i++) {
        for (int j=0; j < (N+2); j++) {
            if (i == 0 || j == 0 || i == (N+1) || j == (N+1)) {
                u[i*(N+2) + j] = g(idx_to_coord(i), idx_to_coord(j));
            } else {
                u[i*(N+2) + j] = 0;
            }
        }
    }
}

// initializes b to the corresponding value of f
void initialize_right_side(float* b) {
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            b[i*(N+2) + j] = f(idx_to_coord(i), idx_to_coord(j));
        }
    }
}

float mean_error(float* u, float* u_exact) {
    float e = 0;
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            e += ( fabs(u[i*(N+2) + j] - u_exact[i*(N+2) + j]) ) / ((N+2)*(N+2));
        }
    }
    return e;
}

void jacobi_step(float* u_new, float* u_old, float* b) {
    float h = 1.0 / (float) N;
    for (int i=1; i <= BLOCK_SIDE; i++) { // perform update on the inner values
        for (int j=1; j <= BLOCK_SIDE; j++) {
            u_new[i*(BLOCK_SIDE+2) + j] = 0.25 * (h*h*b[ i   *(BLOCK_SIDE+2) + j] 
                                                + u_old[(i-1)*(BLOCK_SIDE+2) + j] 
                                                + u_old[(i+1)*(BLOCK_SIDE+2) + j] 
                                                + u_old[ i   *(BLOCK_SIDE+2) + j-1] 
                                                + u_old[ i   *(BLOCK_SIDE+2) + j+1]);
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

void read_block(float* local_u, float* u, int p) { // reads the block of the processor with the given rank from u into local_u
    for (int i=0; i < BLOCK_SIDE; i++) {
        for (int j=0; j < BLOCK_SIDE; j++) {
            local_u[(i+1)*(BLOCK_SIDE+2) + (j+1)] = u[block_start(p) + i*(N+2) + j];
        }
    }
}

void write_block(float* local_u, float* u, int p) { // writes the block of the processor with the given rank from local_u into u
    for (int i=0; i < BLOCK_SIDE; i++) {
        for (int j=0; j < BLOCK_SIDE; j++) {
            u[block_start(p) + i*(N+2) + j] = local_u[(i+1)*(BLOCK_SIDE+2) + (j+1)];
        }
    }
}

void distribute_data_master(float* local_u, float* local_b) {
    float* u = (float*) std::malloc(sizeof(float)*(N+2)*(N+2)); // create unit square with resolution of N*N points plus a border
    initialize_space(u);
    float* b = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    initialize_right_side(b);

    float* tmp_u = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2)*(BLOCK_SIDE+2));
    float* tmp_b = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2)*(BLOCK_SIDE+2));

    for (int p=0; p < NUM_BLOCKS; p++) {
        if (p == MASTER) {
            read_block(local_u, u, p); // Master gets their own data
            read_block(local_b, b, p);
        } else {
            read_block(tmp_u, u, p);
            read_block(tmp_b, b, p);
            MPI_Send(tmp_u, (BLOCK_SIDE+2)*(BLOCK_SIDE+2), MPI_FLOAT, p, 0, MPI_COMM_WORLD); // Master sends block data to other processes
            MPI_Send(tmp_b, (BLOCK_SIDE+2)*(BLOCK_SIDE+2), MPI_FLOAT, p, 0, MPI_COMM_WORLD);
        }
    }
    free(u);
    free(b);
    free(tmp_u);
    free(tmp_b);
}

void distribute_data(float* local_u, float* local_b, int rank) {
    if (rank == MASTER) {
        distribute_data_master(local_u, local_b);
    } else {
        MPI_Recv(local_u, (BLOCK_SIDE+2)*(BLOCK_SIDE+2), MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Processes receive block data from master
        MPI_Recv(local_b, (BLOCK_SIDE+2)*(BLOCK_SIDE+2), MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void read_bottom_border(float* arr, float* vec) { // read the second-to-bottommost row of arr into vec
    for (int j=0; j < BLOCK_SIDE+2; j++) {
        vec[j] = arr[(BLOCK_SIDE)*(BLOCK_SIDE+2) + j];
    }
}

void write_bottom_border(float* arr, float* vec) { // write the contents of vec into the bottommost row of arr
    for (int j=0; j < BLOCK_SIDE+2; j++) {
        arr[(BLOCK_SIDE+1)*(BLOCK_SIDE+2) + j] = vec[j];
    }
}

void read_top_border(float* arr, float* vec) { // read the second-to-topmost row of arr into vec
    for (int j=0; j < BLOCK_SIDE+2; j++) {
        vec[j] = arr[1*(BLOCK_SIDE+2) + j];
    }
}

void write_top_border(float* arr, float* vec) { // write the contents of vec into the topmost row of arr
    for (int j=0; j < BLOCK_SIDE+2; j++) {
        arr[0*(BLOCK_SIDE+2) + j] = vec[j];
    }
}

void read_left_border(float* arr, float* vec) { // read the second-to-leftmost row of arr into vec
    for (int i=0; i < BLOCK_SIDE+2; i++) {
        vec[i] = arr[i*(BLOCK_SIDE+2) + 1];
    }
}

void write_left_border(float* arr, float* vec) { // write the contents of vec into the leftmost row of arr
    for (int i=0; i < BLOCK_SIDE+2; i++) {
        arr[i*(BLOCK_SIDE+2) + 0] = vec[i];
    }
}

void read_right_border(float* arr, float* vec) { // read the second-to-rightmost row of arr into vec
    for (int i=0; i < BLOCK_SIDE+2; i++) {
        vec[i] = arr[i*(BLOCK_SIDE+2) + BLOCK_SIDE];
    }
}

void write_right_border(float* arr, float* vec) { // write the contents of vec into the rightmost row of arr
    for (int i=0; i < BLOCK_SIDE+2; i++) {
        arr[i*(BLOCK_SIDE+2) + BLOCK_SIDE+1] = vec[i];
    }
}


void exchange_borders_vertical(float* local_u, int rank) {
    for (int lower_row=0; lower_row < NUM_BLOCKS_SQRT-1; lower_row++) {
        if (block_row(rank) == lower_row) {
            float* tmp = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2));
            read_bottom_border(local_u, tmp);
            int partner = (block_row(rank)+1)*NUM_BLOCKS_SQRT + block_col(rank); // exchanging values with process in next row
            MPI_Send(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            write_bottom_border(local_u, tmp);
        }
        if (block_row(rank) == lower_row+1) {
            float* tmp = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2));
            int partner = (block_row(rank)-1)*NUM_BLOCKS_SQRT + block_col(rank); // exchanging values with process in previous row
            MPI_Recv(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            write_top_border(local_u, tmp);
            read_top_border(local_u, tmp);
            MPI_Send(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD);
        }
    }
}

void exchange_borders_horizontal(float* local_u, int rank) {
    for (int lower_col=0; lower_col < NUM_BLOCKS_SQRT-1; lower_col++) {
        if (block_col(rank) == lower_col) {
            float* tmp = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2));
            read_right_border(local_u, tmp);
            int partner = block_row(rank)*NUM_BLOCKS_SQRT + block_col(rank)+1; // exchanging values with process in next column
            MPI_Send(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            write_right_border(local_u, tmp);
        }
        if (block_col(rank) == lower_col+1) {
            float* tmp = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2));
            int partner = block_row(rank)*NUM_BLOCKS_SQRT + block_col(rank)-1; // exchanging values with process in previous column
            MPI_Recv(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            write_left_border(local_u, tmp);
            read_left_border(local_u, tmp);
            MPI_Send(tmp, (BLOCK_SIDE+2), MPI_FLOAT, partner, 0, MPI_COMM_WORLD);
        }
    }
}

void exchange_borders(float* local_u, int rank) {
    exchange_borders_vertical(local_u, rank);
    exchange_borders_horizontal(local_u, rank);
}

void swap(float** a, float** b) {
    float* tmp = *a;
    *a = *b;
    *b = tmp;
}

void gather_data_master(float* local_u, float* u, int size) {
    for (int p=0; p < size; p++) {
        float* tmp;
        if (p == MASTER) {
            tmp = local_u;
        } else {
            tmp = (float*) std::malloc(sizeof(float)*(BLOCK_SIDE+2)*(BLOCK_SIDE+2));
            MPI_Recv(tmp, (BLOCK_SIDE+2)*(BLOCK_SIDE+2), MPI_FLOAT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        write_block(tmp, u, p);
    }
}

void gather_data(float* local_u) {
    MPI_Send(local_u, (BLOCK_SIDE+2)*(BLOCK_SIDE+2), MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    float* local_u_old = (float*) std::malloc(sizeof(float) * (BLOCK_SIDE+2)*(BLOCK_SIDE+2)); // Border for values
    float* local_u_new = (float*) std::malloc(sizeof(float) * (BLOCK_SIDE+2)*(BLOCK_SIDE+2)); // that are to be received
    float* local_b     = (float*) std::malloc(sizeof(float) * (BLOCK_SIDE+2)*(BLOCK_SIDE+2)); // from other processesS

    distribute_data(local_u_old, local_b, rank); // each process receives its own block in local_u_old and local_b
    memcpy(local_u_new, local_u_old, sizeof(float)*(BLOCK_SIDE+2)*(BLOCK_SIDE+2)); 

    for (int iteration=0; iteration < NUM_ITER; iteration++) {
        exchange_borders(local_u_old, rank);
        jacobi_step(local_u_old, local_u_new, local_b);
        swap(&local_u_old, &local_u_new); // this iterations u_new becomes next iteration's u_old
    }
    
    float* u; // only used by master
    if (rank == MASTER) {
        u = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
        gather_data_master(local_u_old, u, size);
    } else {
        gather_data(local_u_old);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == MASTER) {
        float* u_exact = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
        fill_in_exact_solution(u_exact);
        //std::cout << mean_error(u, u_exact) << std::endl;
        print_array(u_exact);
        free(u);
        free(u_exact);
    }

    free(local_u_old);
    free(local_u_new);
    free(local_b);
    MPI_Finalize();
    return 0;
}