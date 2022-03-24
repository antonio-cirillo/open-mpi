#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define MAX_INT_VALUE 256

int main(int argc, char* argv[]) {

    MPI_Status status;
    double start, end;
    int rank;
    int size;
    int n;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    if (rank == 0) { // I'm master

        // printf("Inserisci il numero di interi che vuoi inserire:\n");
        // scanf("%d", &size);
        size = atoi(argv[1]);
        printf("Num param: %d\n", size);
        for(int i = 1; i < n; i++) {
            MPI_Send(&size, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
        }

        int buffer[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = rand() % MAX_INT_VALUE;
        }

        for(int i = 1; i < n; i++) {
            MPI_Send(buffer, size, MPI_INT, i, 100, MPI_COMM_WORLD);
        }
    
    } else { // I'm worker
    
        MPI_Recv(&size, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        int buffer[size];

        MPI_Recv(buffer, size, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
        /*printf("[Processo #%d] - Valori ricevuti: [", rank);
        for(int i = 0; i < size - 1; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d]\n", buffer[size - 1]);*/

    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    MPI_Finalize();
    
    if (rank == 0) {
        printf("Time in ms = %f\n", end - start);
    }
    
    return 0;

}