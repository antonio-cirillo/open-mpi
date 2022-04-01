#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define SIZE 20
#define MAX_INT 128

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + size);

    int data[SIZE];

    if (rank == 0) {
        printf("[Processor #0] - Number generate: [ ");
        for (int i = 0; i < SIZE; i++) {
            data[i] = rand() % MAX_INT;
            printf("%d ", data[i]);
        }
        printf("]\n");
    }

    int *sendcounts = malloc((sizeof *sendcounts) * size);
    int *displs = malloc((sizeof *displs) * size);
    int index = 0;

    int rem = SIZE % (size - 1);

    for(int i = 0; i < size; i++) {
        
        if (i == 0) {
            sendcounts[i] = 0;
            continue;
        }

        sendcounts[i] = SIZE / (size - 1);
        if (rem > 0) {
            sendcounts[i]++;
            rem--;
        }

        displs[i] = index;
        index += sendcounts[i];

    }

    int *buffer, *results;
    int count = sendcounts[rank];
    buffer = malloc((sizeof *buffer) * count);
    results = malloc((sizeof *results) * SIZE);

    MPI_Scatterv(data, sendcounts, displs, MPI_INT, buffer, count, MPI_INT, 0, MPI_COMM_WORLD);

    if (0 != rank) {
        printf("[Processor #%d] - Number obtained: [ ", rank);
        for (int i = 0; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("]\n");
    }

    if (0 != rank) {
        int *buffer_copy;
        memcpy(buffer_copy, buffer, (sizeof *buffer) * count);
        for (int i = 1; i < count - 1; i++)
            buffer[i] += buffer_copy[i - 1] + buffer_copy[i + 1];
    }

    MPI_Gatherv(buffer, count, MPI_INT, results, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (0 == rank) {

        for (int i = 1; i < size; i++) {
            int from = displs[i];
            int to = from + sendcounts[i] - 1;

            int between = (from + SIZE - 1) % SIZE;
            int after = (from + 1) % SIZE;
            results[from] += data[between] + data[to];

            between = (to + SIZE - 1) % SIZE;
            after = (to + 1) % SIZE;
            results[to] += data[between] + data[to];
        }

        printf("[Processor #%d] - Results: [ ", rank);
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", results[i]);
        }
        printf("]\n");

    }    

    MPI_Finalize();
    return 0;

}