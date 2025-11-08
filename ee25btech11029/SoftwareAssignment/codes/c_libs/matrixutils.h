
#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
typedef struct Matrix {
    float** data;
    int rows;
    int cols;
} Matrix;
Matrix* create_matrix(int rows, int cols);
void free_matrix(Matrix* mat);
Matrix* copy_matrix(Matrix* A);
float* create_vector(int len);
float* create_random_vector(int len);
float vector_norm(float* v, int len);
void normalize_vector(float* v, int len);
Matrix* matrix_transpose(Matrix* A);
Matrix* matrix_multiply(Matrix* A, Matrix* B);
float* matrix_vector_multiply(Matrix* A, float* v);
Matrix* matrix_subtract(Matrix* A, Matrix* B);
Matrix* matrix_scalar_multiply(Matrix* A, float scalar);
Matrix* vector_outer_product(float* u, float* v, int len_u, int len_v);
#endif
