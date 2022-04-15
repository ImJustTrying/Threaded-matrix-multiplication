/*
 * Kevin Vicente
 * April 2022
 *
 * A testing suite for the Matrix class.
 */

#include <iostream>
#include <cassert>
#include <string>
#include <exception>

#include "matrix.hpp"

// An assertion macro that assures a particular expression does not throw an exception. Credit:
// https://stackoverflow.com/questions/15668309/c-assertion-function-to-check-that-exception-is-thrown
#define ASSERT_NOTHROW(expression) { \
    try {                            \
        expression;                  \
    } catch (...) {                  \
        assert(false);               \
    }                                \
}

#define ASSERT_THROW(expression, exception_type) { \
    try {                                          \
        expression;                                \
        assert(false);                             \
    } catch (const exception_type& e) {            \
    } catch (...) {                                \
        assert(false);                             \
    }                                              \
}


namespace Tests {
    // This first function is a helper for running tests
    void exec_test(auto test_function, std::string test_name) {
        std::cout << "Test: " << test_name << std::endl;
        test_function();
        std::cout << "====== PASSED ======" << std::endl;
    }

    void fields_set() {
        Matrix m(4, 5);
        assert(m.get_height() == 4);
        assert(m.get_width() == 5);
    }

    void equality() {
        Matrix m1(1, 1);
        Matrix m2(1, 1);
        Matrix m3(1, 2);
        m1[0][0] = 1.0f;
        m2[0][0] = 1.0f;
        assert(m1 == m2);
        assert(m1 != m3);
        assert(m2 != m3);
        m1[0][0] = 2.0f;
        assert(m1 != m2);
    }

    void dot_product() {
        Matrix m1(1,2);
        Matrix m2(2,1);
        Matrix product(1,1);
        Matrix expected(1,1);

        m1[0][0] = 1.0f;
        m1[0][1] = 2.0f;
        m2[0][0] = 3.0f;
        m2[1][0] = 4.0f;
        expected[0][0] = 11.0f;

        ASSERT_NOTHROW(m1.nonthreaded_multiply(m2, product));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 1));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 2));
        assert(expected == product);
    }

    void scalar_product() {
        Matrix m1(1,1);
        Matrix m2(1,1);
        Matrix product(1,1);
        Matrix expected(1,1);

        m1[0][0] = 2.0f;
        m2[0][0] = 3.0f;
        expected[0][0] = 6.0f;

        ASSERT_NOTHROW(m1.nonthreaded_multiply(m2, product));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 1));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 2));
        assert(expected == product);
    }

    void bad_dimensions() {
        Matrix m1(1,2);
        Matrix m2(3,1);
        Matrix product(1,1);

        ASSERT_THROW(m1.nonthreaded_multiply(m2, product), std::length_error);
        ASSERT_THROW(m1.threaded_multiply(m2, product, 1), std::length_error);
    }

    void different_dimensions() {
        Matrix m1(1, 2);
        Matrix m2(2, 3);
        Matrix product(1, 3);
        Matrix expected(1, 3);

        m1[0][0] = 1.0f;
        m1[0][1] = 2.0f;
        m2[0][0] = 3.0f;
        m2[0][1] = 4.0f;
        m2[0][2] = 5.0f;
        m2[1][0] = 6.0f;
        m2[1][1] = 7.0f;
        m2[1][2] = 8.0f;
        expected[0][0] = 15.0f;
        expected[0][1] = 18.0f;
        expected[0][2] = 21.0f;

        ASSERT_NOTHROW(m1.nonthreaded_multiply(m2, product));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 1));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 2));
        assert(expected == product);
    }

    void same_dimensions() {
        Matrix m1(2, 2);
        Matrix m2(2, 2);
        Matrix product(2, 2);
        Matrix expected(2, 2);

        m1[0][0] = 1.0f;
        m1[0][1] = 2.0f;
        m1[1][0] = 3.0f;
        m1[1][1] = 4.0f;
        m2[0][0] = 5.0f;
        m2[0][1] = 6.0f;
        m2[1][0] = 7.0f;
        m2[1][1] = 8.0f;
        expected[0][0] = 19.0f;
        expected[0][1] = 22.0f;
        expected[1][0] = 43.0f;
        expected[1][1] = 50.0f;

        ASSERT_NOTHROW(m1.nonthreaded_multiply(m2, product));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 1));
        assert(expected == product);
        ASSERT_NOTHROW(m1.threaded_multiply(m2, product, 2));
        assert(expected == product);
    }
};


int main(void) {
    Tests::exec_test(Tests::fields_set, "matrix fields are set");
    Tests::exec_test(Tests::equality, "matrix equality & inequality correctness");
    Tests::exec_test(Tests::dot_product, "product of 1x2 & 2x1 matrices (vector dot product)");
    Tests::exec_test(Tests::scalar_product, "product of 1x1 matrices (scalar product)");
    Tests::exec_test(Tests::bad_dimensions, "product of 1x2 and 3x1 matrices (invalid dimensions)");
    Tests::exec_test(Tests::different_dimensions, "product of 1x2 and 2x3 matrices (different dimensions)");
    Tests::exec_test(Tests::same_dimensions, "product of 2x2 matrices (same dimensions)");
}
