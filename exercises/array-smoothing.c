#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define N 10
#define K 2

#define MASTER 0
#define K_LAST 100
#define K_FIRST 101

void init_array(int**);
void print_array(int, int*, int, char*);
void send_last_k(int, int, int*, int*);
void send_first_k(int, int, int*, int*);
int recv_last_k(int, int, int*, int**, MPI_Request**);
int recv_first_k(int, int, int*, int**, MPI_Request**);

int main(int argc, char **argv) {

    int rank;                   // Rank of process
    int size;                   // Number of processes
    int *a;                     // Array in input
    int *b;                     // Array smoothed
    int *displs;                // Displacement for gartherv
    int *sendcounts;            // Number of element to send for each worker
    int count;                  // Sub array size
    int *sub_array;             // Sub array for each worker
    int n_worker;               // Number of worker
    int partition;              // Number of partion element
    int rest;                   // Exceed element for partition

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n_worker = size - 1;
    partition = N / n_worker;
    rest = N % n_worker;
    sendcounts = malloc((sizeof *sendcounts) * size);
    sendcounts[0] = 0;
    count = 0;

    if (rank == MASTER) {

        // Init and print array
        init_array(&a);
        print_array(rank, a, N, "Initial array A");

        // Calculate size of all sub-array
        displs = malloc((sizeof *displs) * size);
        int index = 0;
        displs[0] = 0;

        for (int i = 1; i < size; i++) {

            sendcounts[i] = partition;
            if (rest > 0) {
                sendcounts[i]++;
                rest--;
            }

            displs[i] = index;
            index += sendcounts[i];

        }

    } else {

        // Calculate length of all sub-array
        for (int i = 1; i < size; i++) {

            sendcounts[i] = partition;
            if (rest > 0) {
                sendcounts[i]++;
                rest--;
            }
        
        }

        // Allocate memory for recive buffer
        count = sendcounts[rank];
        sub_array = malloc((sizeof *sub_array) * count);

    }

    // Send sub array for each process
    MPI_Scatterv(a, sendcounts, displs, MPI_INT, sub_array, count, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != MASTER) {

        // Send my last K value to next process
        send_last_k(rank, size, sendcounts, sub_array);

        // Send my first K value to prev process
        send_first_k(rank, size, sendcounts, sub_array); 

        // Get last K value to prev process
        MPI_Request *n_recv_last_k_requests;
        int *last_k = calloc(K, sizeof *last_k);
        int n_recv_last_k_request = recv_last_k(rank, size, sendcounts, 
            &last_k, &n_recv_last_k_requests);

        // Get last K value to prev process
        MPI_Request *n_recv_first_k_requests;
        int *first_k = calloc(K, sizeof *first_k);
        int n_recv_first_k_request = recv_first_k(rank, size, sendcounts, 
            &first_k, &n_recv_first_k_requests);

        // Start smoothing on center part 
        int *sub_array_copy = calloc(count, sizeof *sub_array_copy);

        for (int i = 0; i < count; i++) {

            for (int j = i; j <= K + i && j < count; j++)
                sub_array_copy[i] += sub_array[j];

            for (int j = i - 1; j >= 0 && j >= i - K; j--) {
                sub_array_copy[i] += sub_array[j];
            }

        }

        // Smoothing on last k element
        MPI_Waitall(n_recv_last_k_request, n_recv_last_k_requests, NULL);
        for (int i = 0; i < K; i++)
            for (int j = i; j < K; j++)
                sub_array_copy[i] += last_k[j];
        
        // Smoothing on first k element
        MPI_Waitall(n_recv_first_k_request, n_recv_first_k_requests, NULL);
        for (int i = K - 1, dec = 0; i >= 0; i--) {
            for (int j = i; j >= 0; j--) {
                int index = count - 1 - dec;
                sub_array_copy[index] += first_k[j];
            }
            dec++;
        }

        print_array(rank, sub_array_copy, count, "After recv");

    }

    MPI_Finalize();
    return EXIT_SUCCESS;

}

void init_array(int **array) {

    *array = malloc((sizeof *array) * N);
    for (int i = 0; i < N; i++)
        *(*array + i) = i;

}

void print_array(int rank, int *array, int n, char *name) {

    printf("[Processor #%d] - %s: [ ", rank, name);
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("]\n");

}

void send_last_k(int rank, int size, int *sendcounts, int *sub_array) {

    int next = rank + 1;
    if (next == size)
        next = 1;

    int count = sendcounts[rank];

    // I can't satisfy all request, need to send all and delegate request prev process
    if (count < K) {

        ;
        
    } else {
  
        // Send last K element of my sub_array
        MPI_Request request;
        int index = count - K;
        MPI_Isend((sub_array + index), K, MPI_INT, next, K_LAST, MPI_COMM_WORLD, &request);

    }

}

void send_first_k(int rank, int size, int *sendcounts, int *sub_array) {

    int prev = rank - 1;
    if (prev == 0)
        prev = size - 1;

    int count = sendcounts[prev];

    // I can't satisfy all request, need to send all and delegate request prev process
    if (count < K) {

        ;

    } else {
  
        // Send first K element of my sub_array
        MPI_Request request;
        MPI_Isend(sub_array, K, MPI_INT, prev, K_FIRST, MPI_COMM_WORLD, &request);

    }

}

int recv_last_k(int rank, int size, int *sendcounts, int **last_k, MPI_Request **requests) {

    // Number of recv needed
    int n_recv = 0;

    int inc = 1;
    // Get from prev process K element
    for (int i = K; i > 0; inc++) {

        // Get rank of prev worker
        int prev = rank - inc;
        if (prev <= 0)
            prev = size - inc;

        // Number of element that prev process can be send
        int count = sendcounts[prev];

        // Check if prev process can be satisfy request without delegate request to other process
        if (count > K) 
            count = K;
        
        i -= count; // Element recived by prev process
        n_recv++; // Increment number of process needed

    }

    *requests = malloc(sizeof(**requests) * n_recv);
    int n_recv_to_return = n_recv;

    for(inc = 1; n_recv != 0; n_recv--) {

        // Get rank of prev worker
        int prev = rank - inc;
        if (prev <= 0)
            prev = size - inc;

        // Number of element that prev process can be send
        int count = sendcounts[prev];

        // Check if prev process can be satisfy request without delegate request to other process
        if (count > K) 
            count = K;

        MPI_Irecv(*last_k, count, MPI_INT, prev, K_LAST, MPI_COMM_WORLD, *requests);

    }

    return n_recv_to_return;

}

int recv_first_k(int rank, int size, int *sendcounts, int **first_k, MPI_Request **requests) {

    // Number of recv needed
    int n_recv = 0;

    int inc = 1;
    // Get from next process K element
    for (int i = K; i > 0; inc++) {

        // Get rank of next worker
        int next = rank + inc;
        if (next >= size)
            next = inc;

        // Number of element that next process can be send
        int count = sendcounts[next];

        // Check if next process can be satisfy request without delegate request to other process
        if (count > K) 
            count = K;
        
        i -= count; // Element recived by next process
        n_recv++; // Increment number of process needed

    }

    *requests = malloc(sizeof(**requests) * n_recv);
    int n_recv_to_return = n_recv;

    for(inc = 1; n_recv != 0; n_recv--) {

        // Get rank of next worker
        int next = rank + inc;
        if (next >= size)
            next = inc;

        // Number of element that next process can be send
        int count = sendcounts[next];

        // Check if next process can be satisfy request without delegate request to other process
        if (count > K) 
            count = K;

        MPI_Irecv(*first_k, count, MPI_INT, next, K_FIRST, MPI_COMM_WORLD, *requests);

    }

    return n_recv_to_return;

}