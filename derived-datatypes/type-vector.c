#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 4
 
int main(int argc, char** argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    if (size != 4) {
        printf("This application is meant to be run with 4 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    int matrix[SIZE][SIZE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16
    };

    MPI_Datatype column_type;
    MPI_Type_vector(SIZE, 1, SIZE, MPI_INT, &column_type);
    MPI_Type_commit(&column_type);
 
    MPI_Status status;

    if (0 == rank) {

        for (int i = 0; i < SIZE; i++)
            MPI_Send(&matrix[0][i], 1, column_type, i, 1, MPI_COMM_WORLD);

    }

    int buffer[SIZE];
    MPI_Recv(buffer, SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Type_free(&column_type);
    
    printf("[Processor #%d] - Recived message of ColumnType: [ ", rank);
    for (int i = 0; i < SIZE; i++)
        printf("%d ", buffer[i]);
    printf("]\n");

    MPI_Finalize(); 
    return EXIT_SUCCESS;

}