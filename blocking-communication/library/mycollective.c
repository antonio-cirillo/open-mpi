#include <stdio.h>
#include <mpi.h>
#include "mycollective.h"

#define MY_TAG 99


void MPI_Gathering_send(Item* value, MPI_Datatype datatype, MPI_Comm comm, int rank) {
    printf("[Processo #%d] - Invio dato...\n", rank);
    MPI_Send(value, 1, datatype, 0, MY_TAG, comm);
    printf("[Processo #%d] - Valore inviato: ", rank);
    out_item(value);
    printf("\n");
}

void MPI_Gathering_recv(Item buffer[], MPI_Datatype datatype, int np, 
        MPI_Comm comm, MPI_Status* status, int rank) {
    
    for(int i = 1; i < np; i++)
        MPI_Recv(&buffer[i - 1], 1, datatype, i, MY_TAG, comm, status);
    
    printf("[Processo #%d] - Valori ricevuti: ", rank);
    for(int i = 0; i < np - 1; i++) {
        out_item(&buffer[i]);
    }
    printf("\n");

}

