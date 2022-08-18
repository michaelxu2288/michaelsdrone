#include <math.h>
#include <fmt/core.h>
#include <string>

    // struct matrix {
    //     double * array;
    //     unsigned int r, c;

    //     matrix(const unsigned int r,const unsigned int c);
    //     matrix(const double * array, const unsigned int r, const unsigned int c);
    //     ~matrix();

    //     static void multiply(const matrix& r, const matrix& l, matrix& out);
    //     static void add(const matrix& r, const matrix& l, matrix& out);
    //     static void inverse(const matrix& a, matrix& out);
    //     static void transpose(const matrix& a, matrix& out);

            
        // double get();
        // void get(double * data, const std::pair<int, int> upper, const std::pair<int,int> lower);
        // void set(double data, const std::pair<int, int> loc);
        // void set(double * data, const std::pair<int, int> upper, const std::pair<int,int> lower);

    //     matrix operator+ (const matrix& r);
    //     matrix operator* (const matrix& r);
    // }

// static void identity(double * array, const unsigned int r, const unsigned int c) {
//     array
// }

math::matrix::matrix() {
    r = c = 0;
    array = nullptr;
}

math::matrix::matrix(const unsigned int _r, const unsigned int _c) {
    r = _r;
    c = _c;
    array = new double[r*c];
    for(int i = 0; i < r; i ++){
        for(int j = 0; j < c; j ++) {
            array[i * c + j] = i == j;
        }
    }
}

math::matrix::matrix(const double* _array, const unsigned int _r, const unsigned int _c) {
    r = _r;
    c = _c;
    array = new double[r*c];
    for(int i = 0; i < r; i ++){
        for(int j = 0; j < c; j ++) {
            array[i * c + j] = _array[i * c + j];
        }
    }
}

math::matrix::~matrix(){
    delete[] array;
}


void math::matrix::multiply(const matrix& r, const matrix& l, matrix& out) {
    for(int i = 0; i < r.r; i++) {
        for(int j = 0; j < l.c; j ++) {
            int sum = 0;
            for(int k = 0; k < r.c; k ++){
                sum += r.array[i * r.c + k] * l.array[k * l.c + j];
            }
            out.array[i * l.c + j] = sum;
        }
    }
}

void math::matrix::add(const matrix& r, const matrix& l, matrix& out) {
    for(int i = 0; i < r.r; i++) {
        for(int j = 0; j < r.c; j ++) {
            out.array[i * r.c + j] = r.array[i * r.c + j] + l.array[i * l.c + j];
        }
    }
}

double inline math::matrix::set(double value, int _r, int _c) {
    array[_r * c + _c] = value;
}
// static void cholesky_decomp(const math::matrix& a, math::matrix& u, math::matrix& l) {
//     // Decomposing a matrix into Lower Triangular
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j <= i; j++) {
//             int sum = 0;
 
//             if (j == i) // summation for diagonals
//             {
//                 for (int k = 0; k < j; k++)
//                     sum += pow(lower[j][k], 2);
//                 lower[j][j] = sqrt(matrix[j][j] -
//                                         sum);
//             } else {
 
//                 // Evaluating L(i, j) using L(j, j)
//                 for (int k = 0; k < j; k++)
//                     sum += (lower[i][k] * lower[j][k]);
//                 lower[i][j] = (matrix[i][j] - sum) /
//                                       lower[j][j];
//             }
//         }
//     }
// }

void math::matrix::inverse(const matrix& a, matrix& out) {

}

void math::matrix::transpose(const matrix& a, matrix& out) {
    for(int i = 0; i < a.r; i ++) {
        for(int j = 0; j < a.c; j ++) {
            out.array[j * a.r + i] = a.array[i * a.c + j];
        }
    }
}
// #include <iostream>
std::string math::matrix::string(const matrix& a) {
    std::string out = "[\n";
    for(int i = 0; i < a.r; i ++) {
        out += "    ";
        for(int j = 0; j < a.c; j ++) {
            // std::cout << a.array[i * a.c + j]  << " ";
            out += fmt::format("{:10f}, ", a.array[i * a.c + j]);
        }
        out += "\n";
    }
    out += "]";
    // logger::info("{}", s);
    return out;
}