#include "svd.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
SVDResult* compute_truncated_svd(Matrix* A, int k) {
    int m = A->rows;
    int n = A->cols;
    if (k > m || k > n) {
        fprintf(stderr, "k is larger than matrix dimensions.\n");
        return NULL;
    }
    SVDResult* svd=(SVDResult*)malloc(sizeof(SVDResult));
    svd->U = create_matrix(m, k);
    svd->S = create_vector(k);
    svd->V = create_matrix(n, k);
    svd->k = k;
    Matrix* A_T = matrix_transpose(A);
    Matrix* B = matrix_multiply(A_T, A);
    free_matrix(A_T); 
    for (int i = 0; i < k; i++) {
        float* v = create_random_vector(n);
        float lambda = 0.0;
        for (int iter = 0; iter < 30; iter++) {// i am fixing the iterations to 30 
            float* v_new = matrix_vector_multiply(B, v); 
            lambda = vector_norm(v_new, n);
            normalize_vector(v_new, n);
            free(v);
            v = v_new;
        }
        float sigma = sqrt(lambda);
        svd->S[i] = sigma;
        for (int row = 0; row < n; row++) {
            svd->V->data[row][i] = v[row];
        }
        float* u = matrix_vector_multiply(A, v);
        normalize_vector(u, m);  
        for(int row = 0; row < m; row++) {
            svd->U->data[row][i] = u[row];
        }
	// lemme try new methode to decrease the computations ( make a new deflate term)
        Matrix* v_outer = vector_outer_product(v, v, n, n);
        Matrix* B_deflate_term = matrix_scalar_multiply(v_outer, lambda);
        Matrix* B_next = matrix_subtract(B, B_deflate_term);
        free_matrix(B);
        B = B_next; 
        free(v);
        free(u);
        free_matrix(v_outer);
        free_matrix(B_deflate_term);
    }
    free_matrix(B);
    return svd;
}
void free_svd_result(SVDResult* svd) {
    free_matrix(svd->U);
    if (svd->S) {
        free(svd->S);
    }
    free_matrix(svd->V);
    free(svd);
}
