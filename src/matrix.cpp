#include <math.h>



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

math::matrix::matrix(const unsigned int r, const unsigned int c) {
    array = new double[r*c];
}

math::matrix::matrix(const double* _array, const unsigned int r, const unsigned int c) {
    array = new double[r*c];
    for(int i = 0; i < r; i ++){
        for(int j = 0; j < c; j ++) {
            array[i][j] = _array[i][j];
        }
    }
}

math::matrix::~matrix(){
    delete[] array;
}


static void math::matrix::multiply(const matrix& r, const matrix& l, matrix& out) {
    for(int i = 0; i < r.r; i++) {
        for(int j = 0; j < l.c; j ++) {
            int sum = 0;
            for(int k = 0; k < r.c; k ++){
                sum += r.array[i][k] * l.array[k][j];
            }
            out.array[i][j] = sum;
        }
    }
}

static void math::matrix::add(const matrix& r, const matrix& l, matrix& out) {
    for(int i = 0; i < r.r; i++) {
        for(int j = 0; j < r.c; j ++) {
            out.array[i][j] = r.array[i][j] + l.array[i][j];
        }
    }
}

static void cholesky_decomp(const math::matrix& a, math::matrix& u, math::matrix& l) {
    // Decomposing a matrix into Lower Triangular
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            int sum = 0;
 
            if (j == i) // summation for diagonals
            {
                for (int k = 0; k < j; k++)
                    sum += pow(lower[j][k], 2);
                lower[j][j] = sqrt(matrix[j][j] -
                                        sum);
            } else {
 
                // Evaluating L(i, j) using L(j, j)
                for (int k = 0; k < j; k++)
                    sum += (lower[i][k] * lower[j][k]);
                lower[i][j] = (matrix[i][j] - sum) /
                                      lower[j][j];
            }
        }
    }
}

static void math::matrix::inverse(const matrix& a, matrix& out) {

}

static void math::matrix::transpose(const matrix& a, matrix& out) {

}