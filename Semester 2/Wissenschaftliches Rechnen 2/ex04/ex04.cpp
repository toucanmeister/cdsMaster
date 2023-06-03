#include <iostream>
#include <cstdlib>
#include <bsp/bsp.h>
#include <chrono>
#include <cmath>

#define N  100 // N+2 should be divisible by NUM_BLOCKS_SQRT
#define NUM_BLOCKS_SQRT  2
#define NUM_BLOCKS  NUM_BLOCKS_SQRT*NUM_BLOCKS_SQRT // Number of processes has to be NUM_BLOCKS
#define BLOCK_SIDE  (N+2) / NUM_BLOCKS_SQRT
#define FULL_BLOCK_SIZE  (BLOCK_SIDE+2)*(BLOCK_SIDE+2)
#define MASTER  0
#define EPS  1E-9
#define NUM_MEASUREMENTS  10

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

void print_local_array(float* arr) {
    for (int i=0; i < BLOCK_SIDE+2; i++) {
        std::cout << bsp_pid() << " | ";
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

void distribute_data(float* local_u, float* local_b) {
    size_t block_bytes = sizeof(float) * FULL_BLOCK_SIZE;
    bsp_push_reg(local_u, block_bytes); // register memory for communication
    bsp_push_reg(local_b, block_bytes);
    bsp_sync();

    if (bsp_pid() == MASTER) {
        float* u = (float*) std::malloc(sizeof(float)*(N+2)*(N+2)); // create unit square with resolution of N*N points plus a border
        initialize_space(u);
        float* b = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
        initialize_right_side(b);

        float* tmp_u = (float*) std::malloc(block_bytes);
        float* tmp_b = (float*) std::malloc(block_bytes);

        for (int p=0; p < NUM_BLOCKS; p++) {
            if (p == MASTER) {
                read_block(local_u, u, p); // Master gets their own data
                read_block(local_b, b, p);
            } else {
                read_block(tmp_u, u, p);
                read_block(tmp_b, b, p);
                bsp_put(p, tmp_u, local_u, 0, block_bytes); // Master sends block data to other processes
                bsp_put(p, tmp_b, local_b, 0, block_bytes);
            }
        }
        free(u);
        free(b);
        free(tmp_u);
        free(tmp_b);
    }
    bsp_sync();
    bsp_pop_reg(local_u); // deregister memory
    bsp_pop_reg(local_b);
    bsp_sync();
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

void exchange_borders(float* local_u) {
    size_t ghost_size = sizeof(float)*(BLOCK_SIDE+2);
    float* up = (float*) std::malloc(ghost_size);
    float* down = (float*) std::malloc(ghost_size);
    float* left = (float*) std::malloc(ghost_size);
    float* right = (float*) std::malloc(ghost_size);
    float* sendbuf = (float*) std::malloc(ghost_size);
    bsp_push_reg(up, ghost_size);
    bsp_push_reg(down, ghost_size);
    bsp_push_reg(left, ghost_size);
    bsp_push_reg(right, ghost_size);
    bsp_sync();

    if (block_row(bsp_pid()) != 0) {
        int partner = (block_row(bsp_pid())-1)*NUM_BLOCKS_SQRT + block_col(bsp_pid()); // sending values to process above
        read_top_border(local_u, sendbuf);
        bsp_put(partner, sendbuf, down, 0, ghost_size);
    }
    if (block_row(bsp_pid()) != NUM_BLOCKS_SQRT-1) {
        int partner = (block_row(bsp_pid())+1)*NUM_BLOCKS_SQRT + block_col(bsp_pid()); // sending values to process below
        read_bottom_border(local_u, sendbuf);
        bsp_put(partner, sendbuf, up, 0, ghost_size); 
    }
    if (block_col(bsp_pid()) != 0) {
        int partner = block_row(bsp_pid())*NUM_BLOCKS_SQRT + block_col(bsp_pid())-1; // sending values to process to the left
        read_left_border(local_u, sendbuf);
        bsp_put(partner, sendbuf, right, 0, ghost_size);
    }
    if (block_col(bsp_pid()) != NUM_BLOCKS_SQRT-1) {
        int partner = block_row(bsp_pid())*NUM_BLOCKS_SQRT + block_col(bsp_pid())+1; // sending values to process to the right
        read_right_border(local_u, sendbuf);
        bsp_put(partner, sendbuf, left, 0, ghost_size); 
    }
    bsp_sync();
    bsp_pop_reg(up);
    bsp_pop_reg(down);
    bsp_pop_reg(left);
    bsp_pop_reg(right);
    bsp_sync();

    if (block_row(bsp_pid()) != 0) {
        write_top_border(local_u, up);
    }
    if (block_row(bsp_pid()) != NUM_BLOCKS_SQRT-1) {
        write_bottom_border(local_u, down);
    }
    if (block_col(bsp_pid()) != 0) {
        write_left_border(local_u, left);
    }
    if (block_col(bsp_pid()) != NUM_BLOCKS_SQRT-1) {
        write_right_border(local_u, right);
    }
}

void swap(float** a, float** b) {
    float* tmp = *a;
    *a = *b;
    *b = tmp;
}

void gather_data(float* local_u, float* u) {
    bsp_push_reg(local_u, sizeof(float)*FULL_BLOCK_SIZE);
    bsp_sync();
    float* tmpstore[NUM_BLOCKS];
    if (bsp_pid() == MASTER) {
        for (int p=0; p < NUM_BLOCKS; p++) {
            tmpstore[p] = (float*) std::malloc(sizeof(float) * FULL_BLOCK_SIZE);
            bsp_get(p, local_u, 0, tmpstore[p], sizeof(float)*FULL_BLOCK_SIZE);
        }
    }
    bsp_sync();
    bsp_pop_reg(local_u);
    bsp_sync();

    if (bsp_pid() == MASTER) {
        for (int p=0; p < NUM_BLOCKS; p++) {
            write_block(tmpstore[p], u, p);
        }
    }
}

bool is_max_change_below_eps(float* local_u_new, float* local_u_old) {
    float local_max_change = 0;
    for (int i=0; i < FULL_BLOCK_SIZE; i++) {
        float change = fabs(local_u_new[i] - local_u_old[i]);
        if (change > local_max_change) {
            local_max_change = change;
        }
    }

    bsp_push_reg(&local_max_change, sizeof(float));
    bsp_sync();
    
    float recv_changes[NUM_BLOCKS];
    for (int p=0; p < NUM_BLOCKS; p++) {
        bsp_get(p, &local_max_change, 0, &recv_changes[p], sizeof(float)); // get local max changes from other processes
    }
    bsp_sync();

    float max_change = 0;
    for (int p=0; p < NUM_BLOCKS; p++) {
        if (recv_changes[p] > max_change) {
            max_change = recv_changes[p];
        }
    }

    bsp_pop_reg(&local_max_change);
    bsp_sync();
    return max_change < EPS;
}

float* solve_problem() {
    size_t block_bytes = sizeof(float) * FULL_BLOCK_SIZE;
    float* local_u_old = (float*) std::malloc(block_bytes); // Border for values
    float* local_u_new = (float*) std::malloc(block_bytes); // that are to be received
    float* local_b     = (float*) std::malloc(block_bytes); // from other processes

    
    distribute_data(local_u_old, local_b); // each process receives its own block in local_u_old and local_b
    memcpy(local_u_new, local_u_old, sizeof(float)*FULL_BLOCK_SIZE); 
    
    int iterations = 0;
    while(true) {
        iterations++;
        exchange_borders(local_u_old);
        jacobi_step(local_u_old, local_u_new, local_b);
        swap(&local_u_old, &local_u_new); // this iterations u_new becomes next iteration's u_old
        if (is_max_change_below_eps(local_u_old, local_u_new)) break; 
    }
    float* u; // only used by master
    if (bsp_pid() == MASTER) {
        u = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
    }

    gather_data(local_u_old, u);

    if (bsp_pid() == MASTER) {
        //std::cout << "Took " << iterations << " iterations" << std::endl;
    }
    free(local_u_old);
    free(local_u_new);
    free(local_b);
    return u;
}

int main(int argc, char** argv) {
    bsp_begin(NUM_BLOCKS);

    float* u = solve_problem();
    if (bsp_pid() == MASTER) {
        float* u_exact = (float*) std::malloc(sizeof(float)*(N+2)*(N+2));
        fill_in_exact_solution(u_exact);
        //std::cout << mean_error(u, u_exact) << std::endl;
        print_array(u);
        free(u);
        free(u_exact);
    }
    
    bsp_end();
    return 0;
}