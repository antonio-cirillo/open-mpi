#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_INT_VALUE 256

int main(int argc, char* argv[]) {

    MPI_Status status;
    int rank;
    int n;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    if (rank == 0) { // I'm master
    
        int buffer[n - 1];

        for(int i = 1; i < n; i++)
            MPI_Recv(&buffer[i - 1], 1, MPI_INT, i, 99, MPI_COMM_WORLD, &status);

        printf("[Processo #%d] - Valori ricevuti: [", rank);
        for(int i = 0; i < n - 2; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d]\n", buffer[n - 2]);

    } else { // I'm worker
        int number = rank; // rand() % MAX_INT_VALUE;
        MPI_Send(&number, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;

}