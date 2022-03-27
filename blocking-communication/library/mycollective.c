#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "mycollective.h"

void MYMPI_Bcast(void *buf, int count, MPI_Datatype datatype, MPI_Comm comm) {

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

void MYMPI_Scatter(void *sendbuf, int sendcount, void **recvbuf, int *recvcount, 
        MPI_Datatype datatype, int root, MPI_Comm comm) {

    if (datatype != MPI_INT && datatype != MPI_CHAR)
        exit(0);

    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {

        int length = sendcount / (size - 1);
        int rest = sendcount % (size - 1);

        int start = 0;

        for (int i = 0; i < size; i++) {

            if (i == root)
                continue;

            int tmp;
            if (rest > 0) {
                tmp = length + 1;
                rest--;
            } else 
                tmp = length;

            MPI_Send(&tmp, 1, MPI_INT, i, SCATTER_TAG, comm);
            
            if (datatype == MPI_INT)
                MPI_Send(((int* ) sendbuf) + start, tmp, datatype, i, SCATTER_TAG, comm);
            else
                MPI_Send(((char* ) sendbuf) + start, tmp, datatype, i, SCATTER_TAG, comm);

            start += tmp;
            
        }

    } else {

        MPI_Status status;
        MPI_Recv(recvcount, 1, MPI_INT, root, SCATTER_TAG, comm, &status);

        if (datatype == MPI_INT)
            *recvbuf = malloc(sizeof(int) * (*recvcount));
        else
            *recvbuf = malloc(sizeof(char) * (*recvcount));

        MPI_Recv(*recvbuf, *recvcount, datatype, root, SCATTER_TAG, comm, &status);

    }

}

void MYMPI_Gather(void *sendbuf, int sendcount, void **recvbuf, int *recvcount, 
        MPI_Datatype datatype, int root, MPI_Comm comm) {

    if (datatype != MPI_INT && datatype != MPI_CHAR)
        exit(0);

    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank != root) {

        MPI_Send(&sendcount, 1, MPI_INT, root, GATHER_TAG, comm);

        MPI_Send(sendbuf, sendcount, datatype, root, GATHER_TAG, comm);

    } else {

        MPI_Status status;
        int counts[size];
        counts[root] = 0;
        *recvcount = 0;

        for (int i = 0; i < size; i++) {

            if (i == root)
                continue;

            int tmp;
            MPI_Recv(&tmp, 1, MPI_INT, i, GATHER_TAG, comm, &status);
            (*recvcount) += tmp;
            counts[i] = tmp;

        }

        if (datatype == MPI_INT)
            *recvbuf = malloc(sizeof(int) * (*recvcount));
        else
            *recvbuf = malloc(sizeof(char) * (*recvcount));

        int start = 0;

        for (int i = 0; i < size; i++) {

            if (i == root)
                continue;

            if (datatype == MPI_INT)
                MPI_Recv(((int *) *recvbuf) + start, counts[i], datatype, i, 
                    GATHER_TAG, comm, &status);
            else
                MPI_Recv(((char *) *recvbuf) + start, counts[i], datatype, i, 
                    GATHER_TAG, comm, &status);
            
            start += counts[i];

        }

    }

}