/*
 * Kevin Vicente
 *
 * A multithreaded program for multiplying 2 matrices.
 * Given the number of threads and dimension of the matrices, measure the time to multiply them
 * using both non-threaded and threaded procedures.
 * the left matrix is N x M and the right matrix is L x N
 */

#include <iostream>
#include <chrono>
#include <thread>

#include "matrix.hpp"

using std::cout;
using std::endl;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Invalid number command line arguments" << endl;
        cout << "Usage: ./tmatmul num_threads n m l" << endl;
        return 1;
    }

    // This is a linux/macOS exclusive line. Credit:
    // https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
    const unsigned MAX_THREADS = std::thread::hardware_concurrency();
    const unsigned N = atoi(argv[2]);
    const unsigned M = atoi(argv[3]);
    const unsigned L = atoi(argv[4]);
    const unsigned temp = (unsigned)atoi(argv[1]);
    const unsigned NUM_THREADS = (temp < MAX_THREADS) ? temp : MAX_THREADS;
    cout << "Max threads: " << MAX_THREADS << endl;
    cout << "Number of threads: " << NUM_THREADS << endl;
    cout << "Matrix dimensions: ";
    cout << N << "x" << M << ", " << M << "x" << L << ", " << N << "x" << L << endl;

    // initialize matrices
    Matrix matrix1(N,M);
    Matrix matrix2(M,L);
    Matrix product(N,L);
    
    // take no-thread and threaded products, compare time differences
    system_clock::time_point current_time = system_clock::now();
    matrix1.nonthreaded_multiply(matrix2, product);
    duration<float, std::nano> delta_t = system_clock::now() - current_time;
    cout << "Time for no-thread product: " << delta_t.count() / 1.0e6 << " milliseconds" << endl;

    current_time = system_clock::now();
    matrix1.threaded_multiply(matrix2, product, NUM_THREADS);
    delta_t = system_clock::now() - current_time;
    cout << "Time for threaded product: " << delta_t.count() / 1.0e6 << " milliseconds" << endl;

    return 0;
}
