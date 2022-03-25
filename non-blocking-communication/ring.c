#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_INT 101
#define MY_TAG 100
#define START_VALUE 20

int main(int argc, char* argv[]) {

    MPI_Request request;
    MPI_Status status;
    int rank;
    int size;

    int threshold = 1500; // int threshold = atoi(argv[1]);

    int random;
    srand(time(NULL));

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float avg = 0;

    for(int i = 0; i < 10; i++) {

        int total = 0;
        int round = -1;

        if (rank == 0) {
            int start = START_VALUE;
            MPI_Isend(&start, 1, MPI_INT, 1, MY_TAG, MPI_COMM_WORLD, &request);
            printf("[Processor #%d] - Init with: %d\n", rank, start);
            MPI_Wait(&request, &status);
        }

        while(1) {

            MPI_Irecv(&total, 1, MPI_INT, (rank == 0) ? size - 1 : rank - 1,
                MY_TAG, MPI_COMM_WORLD, &request);

            if(rank == 0) {
                round++;
            }

            random = rand() % MAX_INT;

            MPI_Wait(&request, &status);

            if(total == -1) {

                if(rank == 0) {
                    printf("[Processor #%d] - Round(s): %d\n", rank, round);
                    avg += round;
                }

                int exit = -1;            
                MPI_Isend(&exit, 1, MPI_INT, (rank == (size - 1) ? 0 : rank + 1),
                    MY_TAG, MPI_COMM_WORLD, &request);

                break;

            }
        
            printf("[Processor #%d] - %d + %d = ", rank, total, random);
            total += random;
            printf("%d\n", total);
            fflush(stdin);

            if(threshold > total) {

                MPI_Isend(&total, 1, MPI_INT, (rank == (size - 1) ? 0 : rank + 1),
                    MY_TAG, MPI_COMM_WORLD, &request);
            
            } else {
                
                int exit = -1;            
                MPI_Isend(&exit, 1, MPI_INT, (rank == (size - 1) ? 0 : rank + 1),
                    MY_TAG, MPI_COMM_WORLD, &request);
                
            }

        }

        printf("[Processor #%d] - Finish!\n", rank);

    }

    if(rank == 0) {
        printf("Average: %f\n", avg / 10);
    }

    MPI_Finalize();
    return 0;

}