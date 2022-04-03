#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0
#define MAX_LEN 25

int main(int argc, char* argv[]) {
    
    int rank;                    
    int size;                   
    int position;                 

    char buffer[BUFSIZ];       

    float value;
    char name[MAX_LEN]; 
    int param;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER) {
    
        value = 10.0;
        sprintf(name, "My Name");
        param = 1;

        position = 0;
        MPI_Pack(&value, 1, MPI_FLOAT, buffer, BUFSIZ,
            &position, MPI_COMM_WORLD);
        MPI_Pack(name, MAX_LEN, MPI_CHAR, buffer, BUFSIZ,
            &position, MPI_COMM_WORLD);
        MPI_Pack(&param, 1, MPI_INT, buffer, BUFSIZ,
            &position, MPI_COMM_WORLD);

        MPI_Send(buffer, BUFSIZ, MPI_PACKED, 1, 1, MPI_COMM_WORLD);
    
    } else {

        MPI_Recv(buffer, BUFSIZ, MPI_PACKED, 0, 1, MPI_COMM_WORLD, NULL);

        position = 0;
        MPI_Unpack(buffer, BUFSIZ, &position, &value, 1,
            MPI_FLOAT, MPI_COMM_WORLD);
        /* Note that we must know the length of string to expect here!  */
        MPI_Unpack(buffer, BUFSIZ, &position, name, MAX_LEN,
            MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFSIZ, &position, &param, 1,
            MPI_INT, MPI_COMM_WORLD);

        printf("Rank %d:\t%d %.1f %s\n", rank, param, value, name);

  }

  MPI_Finalize();
  return EXIT_SUCCESS;

}