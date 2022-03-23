#include <stdio.h>
#include <mpi.h>
#include "mycollective.h"

#define MY_TAG 99


void MPI_Gathering_send(Item* value, MPI_Datatype datatype, MPI_Comm comm) {
    
    int rank;
    MPI_Comm_rank(comm, &rank);
    
    printf("[Processo #%d] - Invio dato...\n", rank);
    MPI_Send(value, 1, datatype, 0, MY_TAG, comm);
    printf("[Processo #%d] - Valore inviato: ", rank);
    out_item(value);
    printf("\n");

}

void MPI_Gathering_recv(Item buffer[], MPI_Datatype datatype, 
        MPI_Comm comm, MPI_Status* status) {
    
    int np;
    int rank;

    MPI_Comm_size(comm, &np);
    MPI_Comm_rank(comm, &rank);

    for(int i = 1; i < np; i++)
        MPI_Recv(&buffer[i - 1], 1, datatype, i, MY_TAG, comm, status);
    
    printf("[Processo #%d] - Valori ricevuti: ", rank);
    for(int i = 0; i < np - 1; i++) {
        out_item(&buffer[i]);
    }
    printf("\n");

}

void MPI_Broadcast_send(Item* value, int size, MPI_Datatype datatype, MPI_Comm comm) {

    int np;
    int rank;

    MPI_Comm_size(comm, &np);
    MPI_Comm_rank(comm, &rank);

    printf("[Processo #%d] - Comunico il numero di valori...", rank);
    for(int i = 1; i < np; i++) {
        MPI_Send(&size, 1, MPI_INT, i, MY_TAG, comm);
    }
    printf("[Processo #%d] -  Numero di valori comunicato!", rank);

    printf("[Processo #%d] - Comunico i valori...", rank);
    for(int i = 1; i < np; i++) {
        MPI_Send(value, size, datatype, i, MY_TAG, comm);
    }
    printf("[Processo #%d] - Valori comunicati!", rank);

}

void MPI_Broadcast_revc(Item buffer[], MPI_Datatype datatype,
        MPI_Comm comm, MPI_Status* status) {

    int rank;
    MPI_Comm_rank(comm, &rank);

    int size;
    MPI_Recv(&size, 1, MPI_INT, 0, MY_TAG, comm, &status);

    MPI_Recv(buffer, size, datatype, 0, MY_TAG, comm, &status);
    printf("[Processo #%d] - Valori ricevuti: [", rank);
    for(int i = 0; i < size - 1; i++) {
        printf("%d ", buffer[i]);
    }
    printf("%d]\n", buffer[size - 1]);


}