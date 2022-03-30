#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_INT 101
#define MY_TAG 100
#define START_VALUE 0

#define CICLE 3

int main(int argc, char* argv[]) {

    int rank;
    int size;

    int threshold = 400; // int threshold = atoi(argv[1]);

    int random;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    float avg = 0;
    int flag;

    for(int i = 0; i < CICLE; i++) {

        MPI_Request request = MPI_REQUEST_NULL;
        MPI_Status status;

        int total = 0;
        int round = 0;
        flag = 0;


        if (rank == 0) {
            int start = START_VALUE;
            MPI_Isend(&start, 1, MPI_INT, 1, MY_TAG + i, MPI_COMM_WORLD, &request);
            printf("[Processor #%d] - Init with: %d\n", rank, start);
        }

        while(1) {

            MPI_Irecv(&total, 1, MPI_INT, (rank == 0) ? size - 1 : rank - 1,
                MY_TAG + i, MPI_COMM_WORLD, &request);

            if(rank == 0) {
                round++;
            }

            random = 50; // rand() % MAX_INT;

            MPI_Wait(&request, &status);
        
            if (total >= threshold) {

                if (!flag) {
                    MPI_Isend(&total, 1, MPI_INT, (rank == (size - 1) ? 0 : rank + 1),
                        MY_TAG + i, MPI_COMM_WORLD, &request);
                } else {
                    printf("[Processor #%d] - Comunication on tag %d closed!\n", rank, MY_TAG + i);
                }
                
                if (rank == 0) {
                    printf("[Processor #%d] - Round(s): %d\n", rank, round);
                    fflush(stdout);
                    avg += round;
                }

                break;

            }

            printf("[Processor #%d] - %d + %d = ", rank, total, random);
            total += random;
            printf("%d\n", total);
            fflush(stdout);

            if (total >= threshold) {
                flag = 1;
            }
            
            MPI_Isend(&total, 1, MPI_INT, (rank == (size - 1) ? 0 : rank + 1),
                MY_TAG + i, MPI_COMM_WORLD, &request);

        }

        printf("[Processor #%d] - Finish!\n", rank);
        fflush(stdout);

    }

    if(rank == 0) {
        printf("Average: %lf\n", avg / CICLE);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;

}