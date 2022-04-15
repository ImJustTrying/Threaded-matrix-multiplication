/*
 * Kevin Vicente
 * April 2022
 *
 * Header defining the Matrix class.
 * The matrix consists of a h x w 2-D array of floating point numbers.
 */

#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
    public:
        Matrix(unsigned h, unsigned w);
        ~Matrix();
        unsigned get_width();
        unsigned get_height();
        void nonthreaded_multiply(Matrix& mat, Matrix& res);
        void threaded_multiply(Matrix& mat, Matrix& res, unsigned num_threads);
	float* operator[](size_t i);
	bool operator==(Matrix& m);
	bool operator!=(Matrix& m);

    private:
        float** matrix;
        unsigned w;
        unsigned h;
};

#endif
