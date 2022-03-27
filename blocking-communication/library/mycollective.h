#include <mpi.h>

#define BCAST_TAG 100

void MYMPI_Bcast(void* buf, int count, MPI_Datatype datatype, MPI_Comm comm);