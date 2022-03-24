#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include "mycollective.h"

#define MAX_INT_VALUE 256

int main(int argc, char* argv[]) {

    MPI_Status status;
    int rank;
    int size;
    int n;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    if (rank == 0) { // I'm master

        printf("Inserisci il numero di interi che vuoi inserire:\n");
        scanf("%d", &size);

        MPI_Broadcast_send(&size, 1, MPI_INT, MPI_COMM_WORLD);

        int buffer[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = rand() % MAX_INT_VALUE;
        }

        MPI_Broadcast_send(buffer, size, MPI_INT, MPI_COMM_WORLD);

        printf("[Processo #%d] - Valori inviati: [", rank);
        for(int i = 0; i < size - 1; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d]\n", buffer[size - 1]);
    
    } else { // I'm worker
    
        MPI_Broadcast_revc(&size, 1, MPI_INT, MPI_COMM_WORLD, &status);
        int buffer[size];

        MPI_Broadcast_revc(buffer, size, MPI_INT, MPI_COMM_WORLD, &status);

        printf("[Processo #%d] - Valori inviati: [", rank);
        for(int i = 0; i < size - 1; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d]\n", buffer[size - 1]);
        
    }

    MPI_Finalize();
    return 0;

}