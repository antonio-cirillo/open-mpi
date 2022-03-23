#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "item.h"

#define MAX_INT_VALUE 128

int main(int argc, char* argv[]) {

    MPI_Status status;
    int rank;
    int n;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    if (rank == 0) {
    
        Item buffer[n - 1];
        MPI_Gathering_recv(buffer, MPI_INT, n, MPI_COMM_WORLD, &status, rank);
    
    } else {
        
        Item value = rank;
        MPI_Gathering_send(&value, MPI_INT, MPI_COMM_WORLD, rank);

    }

    MPI_Finalize();
    return 0;

}