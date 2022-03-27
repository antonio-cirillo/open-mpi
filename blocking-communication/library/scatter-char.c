#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "mycollective.h"

#define SIZE 5
#define MAX_INT 64

int main(int argc, char** argv) {

    int rank, size;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char buffer[SIZE];
    char *recvbuf;
    int recvcount;

    if (rank == 0) {

        for (int i = 0; i < SIZE; i++)
            buffer[i] = rand() % MAX_INT;

        printf("[Processor #0] - I will send: [ ");
        for (int i = 0; i < SIZE - 1; i++) {
            printf("%c, ", buffer[i]);
        }
        printf("%c ]\n", buffer[SIZE - 1]);

    }

    MYMPI_Scatter(buffer, SIZE, &recvbuf, &recvcount, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank != 0) {

        printf("[Processor #%d] - I recived %d character(s): [ ", rank, recvcount);
        for (int i = 0; i < recvcount - 1; i++) {
            printf("%c, ", recvbuf[i]);
        }
        printf("%c ]\n", recvbuf[recvcount - 1]);

    }

    MPI_Finalize();
    return 0;

}