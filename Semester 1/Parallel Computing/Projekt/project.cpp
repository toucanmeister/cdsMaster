#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <chrono>

using namespace std;

// For both implementations, sendtype = recvtype and sendcount = recvcount always
void naive_scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int size,rank,typesize;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Type_size(sendtype, &typesize);
    
    if (rank == root) {
        for (int p=0; p < size; p++) {
	    long unsigned adr = reinterpret_cast<long unsigned>(sendbuf); // Need to shift this void pointer, so we cast it as an integer
	    adr += sendcount*typesize*p;
	    void* sendbuf_shifted = reinterpret_cast<void*>(adr);
            if (p != root) {
                MPI_Send(sendbuf_shifted, sendcount, sendtype, p, 0, comm);
            } else {
                memcpy(recvbuf, sendbuf_shifted, recvcount*typesize);
            }
        }
    } else {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

// Scatters to all processes from lower to upper (inclusive)
// See the interface method below this
void tree_scatter_rec(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, int lower, int upper, MPI_Comm comm) { 
	int size,rank,typesize;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Type_size(sendtype, &typesize);

	// Trivial case of the recursion
	if (lower == upper) {
		memcpy(recvbuf, sendbuf, recvcount*typesize);
	    return;
	}

	int middle = lower + (upper-lower)/2;
	int dest, sent, lowerroot, upperroot; // Lowerroot and upperroot are the new roots in the next recursive step
	if (root <= middle) { // Root is in the lower half, dest is in the upper half
		dest = middle+1;
		sent = sendcount * (upper-middle);
		lowerroot = root;
		upperroot = dest;
	} else { // Root is in the upper half, dest is in the lower half
		dest = lower;
		sent = sendcount * (middle-lower+1);
		lowerroot = dest;
		upperroot = root;
	}
	
	long unsigned adr = reinterpret_cast<long unsigned>(sendbuf);
	adr += sendcount*(middle-lower+1)*typesize;
	void* sendbuf_shifted = reinterpret_cast<void*>(adr);

	if (root <= middle) {
		if (rank == root) {
			MPI_Send(sendbuf_shifted, sent, sendtype, dest, 0, comm); // Send the latter half of the data to the m+1st process
		}
		if (rank == dest) {
			MPI_Recv(sendbuf_shifted, sent, sendtype, root, 0, comm, MPI_STATUS_IGNORE);	
		}
	} else {
		if (rank == root) {
			MPI_Send(sendbuf, sent, sendtype, dest, 0, comm); // Send the former half of the data to the 0th process
		}
		if (rank == dest) {
			MPI_Recv(sendbuf, sent, sendtype, root, 0, comm, MPI_STATUS_IGNORE);
		}
	}

	if (rank <= middle) {
		tree_scatter_rec(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, lowerroot, lower, middle, comm); // Recurse on lower half
	} else {
		tree_scatter_rec(sendbuf_shifted, sendcount, sendtype, recvbuf, recvcount, recvtype, upperroot, middle+1, upper, comm); // Recurse on upper half
	}
}

void tree_scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int size,rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    tree_scatter_rec(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, 0, size-1, comm);
}

// Test program
int main(int argc, char *argv[]) {
    int M = 4; // Number of elements each process should receive
    int root = 0;
    MPI_Init(&argc, &argv);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double *sendme = (double*) malloc(sizeof(double)*size*M);
    double *receiveme = (double*) malloc(sizeof(double)*M);
	
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == root) {
        for (int i=0; i < size*M; i++) {
            sendme[i] = i; // Test data
        }
    }

    auto t_begin = chrono::high_resolution_clock::now();  
    naive_scatter(sendme, M, MPI_DOUBLE, receiveme, M, MPI_DOUBLE, root, MPI_COMM_WORLD);    
    MPI_Barrier(MPI_COMM_WORLD); // Wait for all processes to be done
    for (int i=0; i < M; i++) {
        //cout << "Naive | Process #" << rank << " received at i=" << i << ": " << receiveme[i] << endl;
        receiveme[i] = 0;
    }    
    auto t_end = chrono::high_resolution_clock::now(); 
    chrono::duration<double, std::milli> time = t_end - t_begin;
    if (rank == root) cout << "Naive: " << time.count() << "ms" << endl;
	
    t_begin = chrono::high_resolution_clock::now();  
    tree_scatter((void*) sendme, M, MPI_DOUBLE,(void*) receiveme, M, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i=0; i < M; i++) {
        //cout << "Tree | Process #" << rank << " received at i=" << i << ": " << receiveme[i] << endl;
        receiveme[i] = 0;
    }
    t_end = chrono::high_resolution_clock::now();
    time = t_end - t_begin;
    if (rank == root) cout << "Tree: " << time.count() << "ms" << endl;
	
    t_begin = chrono::high_resolution_clock::now();  
    MPI_Scatter((void*) sendme, M, MPI_DOUBLE,(void*) receiveme, M, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    t_end = chrono::high_resolution_clock::now();
    time = t_end - t_begin;
    if (rank == root) cout << "MPI: " << time.count() << "ms" << endl;
	
    MPI_Finalize();
    free(sendme);
    free(receiveme);
    return 0;
}
