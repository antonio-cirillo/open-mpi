#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "mycollective.h"

#define SIZE 5
#define MAX_INT 256

int main(int argc, char** argv) {

    int rank, size;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int buffer[SIZE];
    int *recvbuf;
    int recvcount;

    if (rank != 0) {

        for (int i = 0; i < SIZE; i++)
            buffer[i] = rand() % MAX_INT;

        printf("[Processor #%d] - I will send: [ ", rank);
        for (int i = 0; i < SIZE - 1; i++) {
            printf("%d, ", buffer[i]);
        }
        printf("%d ]\n", buffer[SIZE - 1]);

    }

    MYMPI_Gather(buffer, SIZE, &recvbuf, &recvcount, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {

        printf("[Processor #%d] - I recived %d number(s): [ ", rank, recvcount);
        for (int i = 0; i < recvcount - 1; i++) {
            printf("%d, ", recvbuf[i]);
        }
        printf("%d ]\n", recvbuf[recvcount - 1]);

    }

    MPI_Finalize();
    return 0;

}