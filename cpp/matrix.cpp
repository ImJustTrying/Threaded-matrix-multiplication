#include <chrono>
#include <random>
#include <vector>
#include <thread>
#include <exception>

#include "matrix.hpp"

Matrix::Matrix(unsigned h, unsigned w) {
    // allocate memory
    matrix = new float*[h];
    for (unsigned i = 0; i < h; i += 1){ 
        matrix[i] = new float[w];
    }

    // randomize entries
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> dist(0, w);
    for (unsigned i = 0; i < h; i += 1) {
        for (unsigned j = 0; j < w; j += 1) {
            matrix[i][j] = (float)dist(generator);
        }
    }
    this->h = h;
    this->w = w;
}

Matrix::~Matrix() {
    // deallocate every row, then the pointer array
    for (unsigned i = 0; i < h; i += 1) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


unsigned Matrix::get_height() {
    return h;
}

unsigned Matrix::get_width() {
    return w;
}

float* Matrix::operator[](size_t i) {
    return matrix[i];
}

void Matrix::nonthreaded_multiply(Matrix& mat, Matrix& res) {
    if (this->w != mat.get_height()) {
        throw std::length_error("Invalid matrix dimensions");
    }

    for (unsigned i = 0; i < this->h; i += 1) {
        for (unsigned j = 0; j < mat.get_width(); j += 1) {
            res[i][j] = 0;
            for (unsigned k = 0; k < this->w; k += 1) {
                res[i][j] += this->matrix[i][k] * mat[k][j];
            }
        }
    }
}

void Matrix::threaded_multiply(Matrix& mat, Matrix& res, unsigned num_threads = 2) {
    if (this->w != mat.get_height()) {
        throw std::length_error("Invalid matrix dimensions");
    }

    // The function that each thread will be running -- compute the dot product of the row-vectors and
    // column-vectors in mat1 and mat2 repectively with indices in the range [row_min, row_max].
    auto multiply_row_range = [&mat, &res, this](unsigned row_min, unsigned row_max) {
        for (unsigned i = row_min; i <= row_max; i += 1) {
            for (unsigned j = 0; j < mat.get_width(); j += 1) {
                res[i][j] = 0;
                for(unsigned k = 0; k < this->w; k += 1) {
                    res[i][j] += this->matrix[i][k] * mat[k][j];
                }
            }
        }
    };

    unsigned rows_per_thread = this->h / num_threads;
    unsigned threads_with_extra_row = this->h % num_threads;
    std::vector<std::thread> thread_handles;
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
        thread_handles.push_back(std::thread(multiply_row_range, row_min, row_max));
    }

    // wait for threads to complete execution
    for (auto it = thread_handles.begin(); it != thread_handles.end(); it += 1) {
        it->join();
    }
}
