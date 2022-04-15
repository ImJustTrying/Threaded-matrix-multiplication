/*
 * Kevin Vicente
 * April 2022
 *
 * These are tests for the functions implemented in matrix.c.
 */

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "matrix.h"


// utility function to test if dimensions and entires of both matrices are equal
bool equal(matrix* m1, matrix* m2) {
    if (m1->w != m2->w || m1->h != m2->h) {
        return false;
    }
    for (unsigned i = 0; i < m1->h; i += 1) {
        for (unsigned j = 0; j < m1->w; j += 1) {
            if (m1->data[i][j] != m2->data[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void test_dot_product() {
    matrix* matrix1 = allocate_matrix(1,2);
    matrix* matrix2 = allocate_matrix(2,1);
    matrix* product = allocate_matrix(1,1);
    matrix* expected = allocate_matrix(1,1);

    matrix1->data[0][0] = 1.0f;
    matrix1->data[0][1] = 2.0f;
    matrix2->data[0][0] = 3.0f;
    matrix2->data[1][0] = 4.0f;
    expected->data[0][0] = 11.0f;

    assert(nonthreaded_multiply(matrix1, matrix2, product));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 1));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 2));
    assert(equal(expected, product));

    deallocate_matrix(matrix1);
    deallocate_matrix(matrix2);
    deallocate_matrix(product);
    deallocate_matrix(expected);
}

void test_scalar_product() {
    matrix* matrix1 = allocate_matrix(1,1);
    matrix* matrix2 = allocate_matrix(1,1);
    matrix* product = allocate_matrix(1,1);
    matrix* expected = allocate_matrix(1,1);

    matrix1->data[0][0] = 2.0f;
    matrix2->data[0][0] = 3.0f;
    expected->data[0][0] = 6.0f;

    assert(nonthreaded_multiply(matrix1, matrix2, product));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 1));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 2));
    assert(equal(expected, product));

    deallocate_matrix(matrix1);
    deallocate_matrix(matrix2);
    deallocate_matrix(product);
    deallocate_matrix(expected);
}

void test_bad_dimensions() {
    matrix* matrix1 = allocate_matrix(2,3);
    matrix* matrix2 = allocate_matrix(2,4);
    matrix* product = allocate_matrix(3,4);

    assert(!nonthreaded_multiply(matrix1, matrix2, product));

    deallocate_matrix(matrix1);
    deallocate_matrix(matrix2);
    deallocate_matrix(product);
}

void test_same_dimensions() {
    matrix* matrix1 = allocate_matrix(2,2);
    matrix* matrix2 = allocate_matrix(2,2);
    matrix* product = allocate_matrix(2,2);
    matrix* expected = allocate_matrix(2,2);

    matrix1->data[0][0] = 1.0f;
    matrix1->data[0][1] = 2.0f;
    matrix1->data[1][0] = 3.0f;
    matrix1->data[1][1] = 4.0f;
    matrix2->data[0][0] = 5.0f;
    matrix2->data[0][1] = 6.0f;
    matrix2->data[1][0] = 7.0f;
    matrix2->data[1][1] = 8.0f;
    expected->data[0][0] = 19.0f;
    expected->data[0][1] = 22.0f;
    expected->data[1][0] = 43.0f;
    expected->data[1][1] = 50.0f;

    assert(nonthreaded_multiply(matrix1, matrix2, product));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 1));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 2));
    assert(equal(expected, product));

    deallocate_matrix(matrix1);
    deallocate_matrix(matrix2);
    deallocate_matrix(product);
    deallocate_matrix(expected);
}

void test_different_dimensions() {
    matrix* matrix1 = allocate_matrix(1,2);
    matrix* matrix2 = allocate_matrix(2,3);
    matrix* product = allocate_matrix(1,3);
    matrix* expected = allocate_matrix(1,3);

    matrix1->data[0][0] = 1.0f;
    matrix1->data[0][1] = 2.0f;
    matrix2->data[0][0] = 3.0f;
    matrix2->data[0][1] = 4.0f;
    matrix2->data[0][2] = 5.0f;
    matrix2->data[1][0] = 6.0f;
    matrix2->data[1][1] = 7.0f;
    matrix2->data[1][2] = 8.0f;
    expected->data[0][0] = 15.0f;
    expected->data[0][1] = 18.0f;
    expected->data[0][2] = 21.0f;

    assert(nonthreaded_multiply(matrix1, matrix2, product));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 1));
    assert(equal(expected, product));
    assert(threaded_multiply(matrix1, matrix2, product, 2));
    assert(equal(expected, product));

    deallocate_matrix(matrix1);
    deallocate_matrix(matrix2);
    deallocate_matrix(product);
    deallocate_matrix(expected);
}


int main(void) {
    printf("Test: dot product of [1 2] * [3; 4]\n");
    test_dot_product();
    printf("========= PASSED =========\n");

    printf("Test: scalar product of [2] * [3]\n");
    test_scalar_product();
    printf("========= PASSED =========\n");

    printf("Test: failed product of 2x3 and 2x4 matrix\n");
    test_bad_dimensions();
    printf("========= PASSED =========\n");

    printf("Test: product of same dimension (2x2) matrices [1 2; 3 4] * [5 6; 7 8]\n");
    test_same_dimensions();
    printf("========= PASSED =========\n");

    printf("Test: product of different dimension (1x2 and 2x3) matrices [1 2] * [3 4 5; 6 7 8]\n");
    test_different_dimensions();
    printf("========= PASSED =========\n");

    return 0;
}
