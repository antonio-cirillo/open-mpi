#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

    char message[20];    
    int rank;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) { // I'm master
        printf("Inserisci una stringa:\n");
        fgets(message, strlen(message), stdin);
        printf("[Rank #%d] - Invio del messaggio...\n", rank);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
        printf("[Rank #%d] - Messaggio inviato!\n", rank);
    }

    else { // I'm worker
        printf("[Rank #%d] - In attesa del messaggio...\n", rank);
        MPI_Recv(message, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        printf("[Rank #%d] - Messaggio ricevuto: %s.\n", rank, message);
    }

    MPI_Finalize();
    return 0;

}