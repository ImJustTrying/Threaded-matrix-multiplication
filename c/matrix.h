/*
 * Kevin Vicente
 * April 2022
 *
 * Header for the matrix structure.
 * Defines all of the functions that operate on this structure.
 * thread_job_args is just a struct that holds arguments for thread procedures.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>


typedef struct matrix {
    float** data;
    unsigned h;
    unsigned w;
} matrix;

struct thread_job_args {
    matrix* mat1;
    matrix* mat2;
    matrix* res;
    unsigned row_min;
    unsigned row_max;
};


matrix* allocate_matrix(unsigned h, unsigned w);
void deallocate_matrix(matrix* m);
void generate_entries(matrix* m);
bool nonthreaded_multiply(matrix* m1, matrix* m2, matrix* res);
void* multiply_row_range(void* arg);
bool threaded_multiply(matrix* m1, matrix* m2, matrix* res, unsigned num_threads);

#endif
