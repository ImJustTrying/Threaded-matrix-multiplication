#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
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


// allocate memory for NxN matrix
matrix* allocate_matrix(unsigned h, unsigned w) {
    matrix* m = (matrix*) malloc(sizeof(matrix));
    m->data = (float**)malloc(sizeof(float*) * h);
    m->w = w;
    m->h = h;
    for (unsigned i = 0; i < h; i += 1) {
        m->data[i] = (float*)malloc(sizeof(float) * w);
    }
    return m;
}

// deallocate memory for NxN matrix
void deallocate_matrix(matrix* m) {
    for (unsigned i = 0; i < m->h; i += 1) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

// Randomly generate entires for the given matrix using a uniform distribution in the range [0,N]
void generate_entries(matrix* m) {
    srand(time(NULL));
    for (unsigned i = 0; i < m->h; i += 1) {
        for (unsigned j = 0; j < m->w; j += 1) {
            m->data[i][j] = (float)(rand() % m->w);
        }
    }
}

// compute mat1 x mat2 and store the result in the matrix res
bool nothread_multiply(matrix* m1, matrix* m2, matrix* res) {
    if (m1->w != m2->h) {
        return false;
    }
    for (unsigned i = 0; i < m1->h; i += 1) {
        for (unsigned j = 0; j < m2->w; j += 1) {
            res->data[i][j] = 0;
            for (unsigned k = 0; k < m1->w; k += 1) {
                res->data[i][j] += m1->data[i][k] * m2->data[k][j];
            }
        }
    }
    return true;
}

// The function that each thread will be running -- compute the dot product of the row-vectors and
// column-vectors in mat1 and mat2 repectively with indices in the range [row_min, row_max].
void* multiply_row_range(void* arg) {
    struct thread_job_args* args = (struct thread_job_args*) arg;
    for (unsigned i = args->row_min; i <= args->row_max; i += 1) {
        for (unsigned j = 0; j < args->mat2->w; j += 1) {
            args->res->data[i][j] = 0;
            for(unsigned k = 0; k < args->mat1->w; k += 1) {
                args->res->data[i][j] += args->mat1->data[i][k] * args->mat2->data[k][j];
            }
        }
    }
    free(args);
}

// The initial function for the multithreaded matrix multiplication
bool threaded_multiply(matrix* m1, matrix* m2, matrix* res, unsigned num_threads) {
    if (m1->w != m2->h) {
        return false;
    }
    unsigned rows_per_thread = m1->h / num_threads;
    unsigned threads_with_extra_row = m1->h % num_threads;
    pthread_t* thread_handles = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
    int last_row_max = -1;

    for (unsigned i = 0; i < num_threads; i += 1) {
        // for each thread, compute the range of rows in the result matrix it will be computing
        // both row_min and row_max are included in this range
        unsigned row_min = last_row_max + 1;
        unsigned row_max = row_min + rows_per_thread - 1;
        if (i < threads_with_extra_row) {
            row_max += 1;
        }
        last_row_max = row_max;
        struct thread_job_args* arg = (struct thread_job_args*)malloc(sizeof(struct thread_job_args));
        arg->mat1 = m1;
        arg->mat2 = m2;
        arg->res = res;
        arg->row_min = row_min;
        arg->row_max = row_max;
        pthread_create(&thread_handles[i], NULL, multiply_row_range, (void*)arg);
    }

    // wait for threads to complete execution
    for (unsigned i = 0; i < num_threads; i += 1) {
        pthread_join(thread_handles[i], NULL);
    }

    free(thread_handles);
}
