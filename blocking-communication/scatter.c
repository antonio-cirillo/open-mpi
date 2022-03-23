#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_INT_VALUE 256

int main(int argc, char* argv[]) {

    MPI_Status status;
    int rank;
    int size;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) { // I'm Master
        
        int n;
        printf("Quanti numeri vuoi generare?\n");
        scanf("%d", &n);

        int buffer[n];
        for(int i = 0; i < n; i++) {
            buffer[i] = rand() % MAX_INT_VALUE;
        }

        printf("[Processo #%d] - %d valori generati: [", rank, n);
        for(int i = 0; i < n - 1; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d]\n", buffer[n - 2]);

        int mod = n % (size - 1);
        int _n = n / (size - 1);
        int start = 0;

        for(int i = 1; i < size; i++) {

            if ((i - 1) < mod) {
                
                int _buffer[_n + 1];
                int _n_tmp = _n + 1;
                // Comunico la grandezza del buffer
                MPI_Send(&_n_tmp, 1, MPI_INT, i, 99, MPI_COMM_WORLD);

                for(int j = 0; j < _n_tmp; j++)
                    _buffer[j] = buffer[start + j];

                MPI_Send(_buffer, _n_tmp, MPI_INT, i, 100, MPI_COMM_WORLD);

                start += _n_tmp;

            } else {

                int _buffer[_n];
                // Comunico la grandezza del buffer
                MPI_Send(&_n, 1, MPI_INT, i, 99, MPI_COMM_WORLD);

                for(int j = 0; j < _n; j++)
                    _buffer[j] = buffer[start + j];

                MPI_Send(_buffer, _n, MPI_INT, i, 100, MPI_COMM_WORLD);

                start += _n;

            }

        }

    } else { // I'm worker 
    
        int n;
        MPI_Recv(&n, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        printf("[Processo #%d] - Valore ricevuto: %d\n", rank, n);
        
        int buffer[n];
        MPI_Recv(buffer, n, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
        printf("[Processo #%d] - Valori ricevuti: [", rank);
        for(int i = 0; i < n - 1; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d]\n", buffer[n - 1]);

    }

    MPI_Finalize();
    return 0;

}