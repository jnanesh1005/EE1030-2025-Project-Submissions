#include "matrixutils.h"
Matrix* create_matrix(int rows, int cols) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix)); 
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (float**)malloc(rows * sizeof(float*));
    if (mat->data == NULL) {
        printf("Error.\n");
        free(mat);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        mat->data[i] = (float*)malloc(cols * sizeof(float));
        if (mat->data[i] == NULL) {
            printf("Error: Could not allocate memory for row %d.\n", i);
            for (int j = 0; j < i; j++) {
                free(mat->data[j]);
            }
            free(mat->data); 
            free(mat);       
            return NULL;
        }
        memset(mat->data[i], 0, cols * sizeof(float));
    }
    return mat;
}

void free_matrix(Matrix* mat) {
    if (mat == NULL) {
        return; 
    }
    for (int i = 0; i < mat->rows; i++) {
        if(mat->data[i] != NULL) {
             free(mat->data[i]);
        }
    }
    if(mat->data != NULL) {
        free(mat->data);
    }
    free(mat);
}

Matrix* copy_matrix(Matrix* A){
    if(A==NULL){
        return NULL;
    }
    Matrix* copy = create_matrix(A->rows, A->cols);
    if(copy==NULL){
        return NULL;
    }
    for(int i = 0; i < A->rows; i++){
        for(int j = 0; j < A->cols; j++){
            copy->data[i][j] = A->data[i][j];
        }
    }
    return copy;
}

float* create_vector(int len){
    float* vec = (float*)malloc(len * sizeof(float));
    if(vec == NULL){
        // We failed to allocate, so we must return NULL.
        // We can't memset(NULL, ...), that's a crash!
        return NULL; 
    }
    
    // --- THIS IS THE FIX ---
    // The memset must be OUTSIDE the 'if' block.
    memset(vec, 0, len * sizeof(float)); 
    return vec;
}

float* create_random_vector(int len) {
    // create_vector now correctly returns a zero-initialized vector
    float* vec = create_vector(len); 
    if (vec == NULL) { return NULL; }
    
    for (int i = 0; i < len; i++) {
        // We can't just use rand(), we must seed it in main()
        vec[i] = (float)rand() / (float)RAND_MAX;
    }
    
    // It's good practice to normalize the random vector
    normalize_vector(vec, len); 
    return vec;
}

float vector_norm(float* v, int len) {
    if (v == NULL) return 0.0; // Safety check
    float sum_of_squares = 0.0;
    for (int i = 0; i < len; i++) {
        sum_of_squares += v[i] * v[i];
    }
    return sqrt(sum_of_squares);
}

void normalize_vector(float* v, int len) {
    if (v == NULL) return; // Safety check
    float norm = vector_norm(v, len);
    
    // Check for division by zero
    if (norm > 1e-6) { 
        float inv_norm = 1.0 / norm;
        for (int i = 0; i < len; i++) {
            v[i] = v[i] * inv_norm;
        }
    }
}

Matrix* matrix_transpose(Matrix* A) {
    if (A == NULL) { return NULL; }
    Matrix* A_T = create_matrix(A->cols, A->rows);
    if (A_T == NULL) { return NULL; }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            A_T->data[j][i] = A->data[i][j];
        }
    }
    return A_T;
}

Matrix* matrix_multiply(Matrix* A, Matrix* B) {
    if (A == NULL || B == NULL || A->cols != B->rows) {
        return NULL;
    }
    int m = A->rows;
    int n = A->cols;
    int p = B->cols;
    Matrix* C = create_matrix(m, p);
    if (C == NULL) { return NULL; }
    for (int i = 0; i < m; i++) {       
        for (int j = 0; j < p; j++) {  
            // C[i][j] is already 0.0 from create_matrix
            for (int k = 0; k < n; k++) {
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }
    return C;
}

float* matrix_vector_multiply(Matrix* A, float* v) {
    if (A == NULL || v == NULL) { return NULL; }
    int m = A->rows;
    int n = A->cols;
    float* y = create_vector(m); 
    if (y == NULL) { return NULL; }
    
    // y is already initialized to all zeros
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            y[i] += A->data[i][j] * v[j];
        }
    }
    return y;
}

Matrix* matrix_subtract(Matrix* A, Matrix* B) {
    if (A == NULL || B == NULL || A->rows != B->rows || A->cols != B->cols) {
        return NULL;
    }
    Matrix* C = create_matrix(A->rows, A->cols);
    if (C == NULL) { return NULL; }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            C->data[i][j] = A->data[i][j] - B->data[i][j];
        }
    }
    return C;
}

Matrix* matrix_scalar_multiply(Matrix* A, float scalar) {
    if (A == NULL) { return NULL; }
    Matrix* C = create_matrix(A->rows, A->cols);
    if (C == NULL) { return NULL; }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            C->data[i][j] = A->data[i][j] * scalar;
        }
    }
    return C;
}

Matrix* vector_outer_product(float* u, float* v, int len_u, int len_v) {
    if (u == NULL || v == NULL) { return NULL; }
    Matrix* A = create_matrix(len_u, len_v);
    if (A == NULL) { return NULL; }

    for (int i = 0; i < len_u; i++) {
        for (int j = 0; j < len_v; j++) {
            A->data[i][j] = u[i] * v[j];
        }
    }
    return A;
}
