#include <mpi.h>

#define BCAST_TAG 100
#define SCATTER_TAG 101
#define GATHER_TAG 102

void MYMPI_Bcast(void* buf, int count, MPI_Datatype datatype, MPI_Comm comm);

void MYMPI_Scatter(void *sendbuf, int sendcount, void **recvbuf, int *recvcount, 
        MPI_Datatype datatype, int root, MPI_Comm comm);

void MYMPI_Gather(void *sendbuf, int sendcount, void **recvbuf, int *recvcount,
        MPI_Datatype datatype, int root, MPI_Comm comm);