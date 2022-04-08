/*
 * Kevin Vicente
 *
 * A multithreaded program for multiplying 2 matrices.
 * Given the number of threads and dimension of the matrices, measure the time to multiply them
 * using both non-threaded and threaded procedures.
 * the left matrix is N x M and the right matrix is L x N
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "matrix.h"


int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Invalid number command line arguments\n");
        printf("Usage: ./tmatmul num_threads n m l\n");
        return 1;
    }

    // This is a linux/macOS exclusive line. Credit:
    // https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
    const unsigned MAX_THREADS = (unsigned)sysconf(_SC_NPROCESSORS_ONLN);
    const unsigned N = atoi(argv[2]);
    const unsigned M = atoi(argv[3]);
    const unsigned L = atoi(argv[4]);
    const unsigned temp = (unsigned)atoi(argv[1]);
    const unsigned NUM_THREADS = (temp < MAX_THREADS) ? temp : MAX_THREADS;
    printf("Max threads: %i\n", MAX_THREADS);
    printf("Number of threads: %i\n", NUM_THREADS);
    printf("Matrix dimension: %ix%i, %ix%i, %ix%i\n", N, M, M, L, N, L);

    // allocate and initialize matrices
    matrix* matrix1 = allocate_matrix(N,M);
    matrix* matrix2 = allocate_matrix(M,L);
    matrix* product = allocate_matrix(N,L);
    generate_entries(matrix1);
    generate_entries(matrix2);
    
    // take no-thread and threaded products, compare time differences
    clock_t current_time = clock();
    nothread_multiply(matrix1, matrix2, product);
    float delta_t = (float)(clock() - current_time) / CLOCKS_PER_SEC * 1.0e3;
    printf("Time for no-thread product: %f milliseconds\n", delta_t);

    current_time = clock();
    threaded_multiply(matrix1, matrix2, product, NUM_THREADS);
    delta_t = (float)(clock() - current_time) / CLOCKS_PER_SEC * 1.0e3;
    printf("Time for threaded product: %f milliseconds\n", delta_t);

    // free allocated memory
    deallocate_matrix(matrix1);
    deallocate_matrix(matrix2);
    deallocate_matrix(product);
    return 0;
}
