
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "matrixutils.h"
#include "svd.h"
#include "analysis.h"
struct Matrix* reconstruct_image(struct SVDResult* svd){
    struct Matrix* U = svd->U;
    float* S_array = svd->S;
    struct Matrix* V = svd->V;
    int k=V->cols; 
    struct Matrix* Sigma_k = create_matrix(k, k);
    for (int i = 0; i < k; i++) {
        Sigma_k->data[i][i] = S_array[i];
    }
    struct Matrix* V_T = matrix_transpose(V);
    struct Matrix* temp = matrix_multiply(U, Sigma_k);
    struct Matrix* A_k = matrix_multiply(temp, V_T);
    free_matrix(Sigma_k);
    free_matrix(V_T);
    free_matrix(temp);
    return A_k;
}
float compute_frobenius_error(struct Matrix* A, struct Matrix* A_k){
    struct Matrix* diff = matrix_subtract(A, A_k);
    double sum_of_squares = 0.0; 
    for (int i = 0; i < diff->rows; i++) {
        for (int j = 0; j < diff->cols; j++) {
            float d = diff->data[i][j];
            sum_of_squares += (double)d * (double)d;
        }
    }
    free_matrix(diff);
    return (float)sqrt(sum_of_squares);
}
void write_singular_values(const char* filename, SVDResult* svd){
    FILE* file = fopen(filename, "w");
    for (int i = 0; i < svd->k; i++) {
        fprintf(file, "%f\n", svd->S[i]);
    }
    fclose(file);
    printf("singular values in %s\n", filename);
}
