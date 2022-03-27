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

void MYMPI_Gather(void* sendbuf, int sendcount, int* recvbuf, int* recvcount, int root,
        MPI_Datatype datatype, MPI_Comm comm) {

    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank != root) {

        MPI_Send(&sendcount, 1, MPI_INT, root, GATHER_TAG, comm);
        MPI_Send(sendbuf, sendcount, datatype, root, GATHER_TAG, comm);

    } else {
        
        MPI_Status status;

        *recvcount = 0;
        int counts[size];
        counts[root] = 0;

        for (int i = 0; i < size; i++) {

            int count;

            if (i == root)
                continue;
            
            MPI_Recv(&count, 1, MPI_INT, i, GATHER_TAG, comm, &status);
            *recvcount += count;
            counts[i] = count;

        }

        if (datatype == MPI_INT)
            recvbuf = malloc(*recvcount * sizeof(int));
        else if (datatype == MPI_CHAR)
            recvbuf = malloc(*recvcount * sizeof(char));

        int index = 0;

        for (int i = 0; i < size; i++) {
            
            if (i == root)
                continue;

            printf("Recvbuf: %p\nIndex: %d\nRecbuf + index: %p\n", recvbuf, index, recvbuf + index);
            MPI_Recv(recvbuf + index, counts[i], datatype, i, GATHER_TAG, comm, &status);
            index += counts[i];

        }
    
        int *a = recvbuf;
        for (int i = 0; i < 9; i++) {
            printf("%d ", a[i]);
        }

    }

}