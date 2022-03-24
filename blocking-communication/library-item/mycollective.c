#include <stdio.h>
#include <mpi.h>
#include "mycollective.h"

#define MY_TAG 99


void MPI_Gathering_send(Item* value, MPI_Datatype datatype, MPI_Comm comm) {
       
    MPI_Send(value, 1, datatype, 0, MY_TAG, comm);

}

void MPI_Gathering_recv(Item buffer[], MPI_Datatype datatype, 
        MPI_Comm comm, MPI_Status* status) {
    
    int np;
    int rank;

    for(int i = 1; i < np; i++)
        MPI_Recv(&buffer[i - 1], 1, datatype, i, MY_TAG, comm, status);
    
}

void MPI_Broadcast_send(Item* value, int size, MPI_Datatype datatype, MPI_Comm comm) {

    int rank;
    int n;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    for(int i = 1; i < n; i++) {
        MPI_Send(value, size, datatype, i, MY_TAG, comm);
    }

}

void MPI_Broadcast_revc(Item buffer[], int size, MPI_Datatype datatype,
        MPI_Comm comm, MPI_Status* status) {

    MPI_Recv(buffer, size, datatype, 0, MY_TAG, comm, &status);

}