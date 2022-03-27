#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "mycollective.h"

void MYMPI_Bcast(void* buf, int count, MPI_Datatype datatype, MPI_Comm comm) {

    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == 0) {

        for (int i = 1; i < size; i++)
            MPI_Send(buf, count, datatype, i, BCAST_TAG, comm);

    } else {

        MPI_Status status;
        MPI_Recv(buf, count, datatype, 0, BCAST_TAG, comm, &status);

    }

}
