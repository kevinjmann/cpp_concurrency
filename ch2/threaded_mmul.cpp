#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

void dotproduct(const double mat1[], int nrow_1, int ncol_1, const double mat2[], int nrow_2, int ncol_2, double result[], int target_row) {
    for (int i = 0; i < ncol_2; i++) {
        for (int j = 0; j < nrow_2; j++) {
            result[target_row * ncol_2 + i] += mat1[target_row * ncol_1 + j] * mat2[i + ncol_2*j];
        }
    }
}
void matmul(const double mat1[], int nrow_1, int ncol_1, const double mat2[], int nrow_2, int ncol_2, double result[]) {
    std::vector<std::thread> threads;
    for(int i = 0; i < nrow_1; i++) {
        threads.push_back(std::thread{dotproduct, mat1, nrow_1, ncol_1, mat2, nrow_2, ncol_2, result, i});
    }
    for(auto& t : threads) {
        t.join();
    }
}

void matmul_serial(const double mat1[], int nrow_1, int ncol_1, const double mat2[], int nrow_2, int ncol_2, double result[]) {
    for(int i =0; i < nrow_1; i++) {
        dotproduct(mat1, nrow_1, ncol_1, mat2, nrow_2, ncol_2, result, i);
    }
}

void matmul_chunk(const double mat1[], int nrow_1, int ncol_1, const double mat2[], int nrow_2, int ncol_2, double result[], int row_start, int row_end) {
    for (int i = row_start; i < row_end; i++) {
        dotproduct(mat1, nrow_1, ncol_1, mat2, nrow_2, ncol_2, result, i);
    }
}

void matmul_balanced(const double mat1[], int nrow_1, int ncol_1, const double mat2[], int nrow_2, int ncol_2, double result[]) {
    if (nrow_1 == 1) {
        matmul_serial(mat1, nrow_1, ncol_1, mat2, nrow_2, ncol_2, result);
        return;
    }
    int hardware_threads = std::thread::hardware_concurrency();
    int num_threads = std::min(hardware_threads, nrow_1);
    int interval = nrow_1 / num_threads;
    interval = std::max(interval, 1);
    std::vector<std::thread> threads;
    threads.reserve(num_threads - 1);
    int row_start = 0;
    int row_end = interval;
    for (int i = 0; i < (num_threads -1 ); i++) {
        threads.emplace_back(matmul_chunk, mat1, nrow_1, ncol_2, mat2, nrow_2, ncol_2, result, row_start, row_end);
        row_start = row_end;
        row_end += interval;
    }
    matmul_chunk(mat1, nrow_1, ncol_1, mat2, nrow_2, ncol_2, result, row_start, nrow_1);
    for (auto& t : threads){
        t.join();
    }
    
}

int main() {
    // Test random matrix multiplication
    // Results
    // +---------+--------------------+-------------------+
    // |  Dims   | parallel speed(us) | serial speed (us) |
    // +---------+--------------------+-------------------+
    // | 2x3*3x2 |                178 |                 2 |
    // | 10x10   |                426 |                 7 |
    // | 100x100 |               2893 |              5224 |
    // +---------+--------------------+-------------------+
    // only after a significant number of rows did the benefit of threading
    // overcome the overhead costs of thread creation.
    
    double mat1[10000];
    double mat2[10000];
    double mat3[10000];
    double mat4[10000];
    double mat5[10000];
    double mat6[10000];
    for(int i = 0; i < 10000; i++) {
        mat1[i]=std::rand()%10000;  //Generate number between 0 to 99
        mat2[i] = std::rand()%10000;
        mat3[i] = std::rand()%10000;
        mat4[i] = std::rand()%10000;
        mat5[i] = std::rand()%10000;
        mat6[i] = std::rand()%10000;
    }

    double result[10000];
    double resultSerial[10000];
    double resultBalanced[10000];
    auto initial = std::chrono::high_resolution_clock::now();
    matmul(mat1, 100, 100, mat2, 100, 100, result);
    auto parallel_done = std::chrono::high_resolution_clock::now();
    matmul_serial(mat3, 100, 100, mat4, 100, 100, resultSerial);
    auto serial_done = std::chrono::high_resolution_clock::now();
    matmul_balanced(mat5, 100, 100, mat6, 100, 100, resultBalanced);
    auto balanced_done = std::chrono::high_resolution_clock::now();
    auto duration_concurrent = std::chrono::duration_cast<std::chrono::microseconds>(parallel_done - initial);
    auto duration_serial = std::chrono::duration_cast<std::chrono::microseconds>(serial_done - parallel_done);
    auto duration_balanced = std::chrono::duration_cast<std::chrono::microseconds>(balanced_done - serial_done);
    
    std::cout << "parallel time: " << duration_concurrent.count() << std::endl;
    std::cout << "serial time: " << duration_serial.count() << std::endl;
    std::cout << "balanced time: "<< duration_balanced.count() << std::endl;
}