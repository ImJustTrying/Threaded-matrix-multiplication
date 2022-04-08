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

    private:
        float** matrix;
        unsigned w;
        unsigned h;
};

#endif
