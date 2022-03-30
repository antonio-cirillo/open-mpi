#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MY_TAG 100

int main(int argc, char **argv) {

    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total = rank;
    int source = (rank + 1) % size;
    int destination = (rank + size - 1) % size;

    if (rank == 0)
        MPI_Send(&total, 1, MPI_INT, destination, MY_TAG, MPI_COMM_WORLD);

    int tmp;
    MPI_Recv(&tmp, 1, MPI_INT, source, MY_TAG, MPI_COMM_WORLD, &status);
    total += tmp;
    
    if (rank != 0)
        MPI_Send(&total, 1, MPI_INT, destination, MY_TAG, MPI_COMM_WORLD);
    else {
        printf("Total is equal to: %d\n", total);
    }

    MPI_Finalize();
    return 0;

}