#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

struct thread_job_args;
struct matrix;
typedef struct matrix matrix;

void generate_entries(matrix* m);
matrix* allocate_matrix(unsigned w, unsigned h);
void deallocate_matrix(matrix* m);
bool nothread_multiply(matrix* m1, matrix* m2, matrix* res);
void* multiply_row_range(void* arg);
bool threaded_multiply(matrix* m1, matrix* m2, matrix* res, unsigned num_threads);

#endif
