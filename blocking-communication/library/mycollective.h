#include "item.h"

void MPI_Gathering_send(Item* value, MPI_Datatype datatype, MPI_Comm comm);

void MPI_Gathering_recv(Item buffer[], MPI_Datatype datatype,
        MPI_Comm comm, MPI_Status* status);

void MPI_Broadcast_send(Item* value, int size, MPI_Datatype datatype, MPI_Comm comm);

void MPI_Broadcast_revc(Item buffer[], MPI_Datatype datatype,
        MPI_Comm comm, MPI_Status* status);