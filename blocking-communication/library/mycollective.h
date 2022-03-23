#include "item.h"

void MPI_Gathering_send(Item* value, MPI_Datatype datatype, MPI_Comm comm, int rank);

void MPI_Gathering_recv(Item buffer[], MPI_Datatype datatype, int np, 
    MPI_Comm comm, MPI_Status* status, int rank);