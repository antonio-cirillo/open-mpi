#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mycollective.h"

#define SIZE 10
#define MAX_NUMBER 128

int main(int argc, char** argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char buffer[SIZE];
    srand(time(NULL));

    if (rank == 0) { // I'm master

        for (int i = 0; i < SIZE; i++)
            buffer[i] = rand() % MAX_NUMBER;

        printf("[Processor #0] - I will send: [");
        for (int i = 0; i < SIZE - 1; i++) {
            printf("%c ", buffer[i]);
        }
        printf("%c]\n", buffer[SIZE - 1]);

    }

    MYMPI_Bcast(&buffer, SIZE, MPI_CHAR, MPI_COMM_WORLD);

    if (rank != 0) {

        printf("[Processor #%d] - I recived: [", rank);
        for (int i = 0; i < SIZE - 1; i++) {
            printf("%c ", buffer[i]);
        }
        printf("%c]\n", buffer[SIZE - 1]);

    }

    MPI_Finalize();
    return 0;

}