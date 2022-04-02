#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define PI25DT 3.141592653589793238462643

int main(int argc, char **argv) {

    MPI_Status status;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    double sum, dx, x, f, mypi, pi;

    if (0 == rank) {
        printf("Enter the number of intervals: (0 quits): ");
        fflush(stdout);
        scanf("%d", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n <= 0)
        exit(1);

    dx = 1.0 / (double) n;
    sum = 0.0;

    for (int i = rank + 1; i <= n; i += size) {
        x = dx * ((double) (i - 0.5));
        f = 4.0 / (1.0 + x * x);
        sum += f;
    }

    mypi = dx * sum;

    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Pi is approximately %.16f\nError is %.16f\n", pi, fabs(pi - PI25DT));
    }

    MPI_Finalize();
    return 0;

}