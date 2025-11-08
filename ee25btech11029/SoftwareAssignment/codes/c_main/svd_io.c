#include "svd_io.h"
#include <stdio.h>
#include <stdlib.h>

void write_compressed_svd(const char* filename, SVDResult* svd) {
    FILE* file = fopen(filename, "w");
    int m=svd->U->rows;
    int n=svd->V->rows; 
    int k= svd->k;
    fprintf(file, "%d %d %d\n", m, n, k);
    for (int i = 0; i < k; i++) {
        fprintf(file, "%g ", svd->S[i]);
    }
    fprintf(file, "\n"); 
    for (int i=0;i<m;i++) {
        for (int j=0;j<k;j++){
            fprintf(file, "%g ", svd->U->data[i][j]);
        }
        fprintf(file, "\n"); 
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < k; j++){
            fprintf(file, "%g ", svd->V->data[i][j]);
        }
        fprintf(file, "\n"); 
    }
    fclose(file);
    printf("compression is done on(yay!)%s\n", filename);
}
SVDResult* read_compressed_svd(const char* filename){
    FILE* file = fopen(filename, "r");
    int m, n, k;
    if (fscanf(file, "%d %d %d", &m, &n, &k) != 3) {
        fprintf(stderr, "the header is wronf.\n");
        fclose(file);
        return NULL;
    }
    SVDResult* svd = (SVDResult*)malloc(sizeof(SVDResult));
    if (svd == NULL) { return NULL; }
    svd->U = create_matrix(m, k);
    svd->S = create_vector(k); 
    svd->V = create_matrix(n, k);
    svd->k = k;
    for (int i = 0; i < k; i++) {
        if (fscanf(file, "%f", &svd->S[i]) != 1) {
             fprintf(stderr, " no vector S.\n");
             free_svd_result(svd);
             fclose(file);
             return NULL;
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            if (fscanf(file, "%f", &svd->U->data[i][j]) != 1) {
                fprintf(stderr, "no U matrix.\n");
                free_svd_result(svd);
                fclose(file);
                return NULL;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            if (fscanf(file, "%f", &svd->V->data[i][j]) != 1) {
                fprintf(stderr, "no V matrix.\n");
                free_svd_result(svd);
                fclose(file);
                return NULL;
            }
        }
    }
    fclose(file);
    printf("its done on  %s\n", filename);
    return svd;
}
