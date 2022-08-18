#include <math.h>
#include <logger.h>

int main() {
    math::matrix mat(2,2);
    logger::info("Created Matrix");
    logger::info("Matrix: \n{}", math::matrix::string(mat));
    mat.set(2, 0, 0);
    mat.set(1, 1, 0);
    math::matrix mat2(2,1), mat3(2,1);
    mat2.set(2, 0, 0);
    mat2.set(1, 0, 1);
    math::matrix::multiply(mat, mat2, mat3);

    logger::info("Matrix2: \n{}", math::matrix::string(mat2));
    logger::info("Matrix3: \n{}", math::matrix::string(mat3));
}