
#ifndef SVD_H
#define SVD_H
#include "matrixutils.h"
typedef struct SVDResult {
    Matrix* U;  
    float* S;   
    Matrix* V;  
    int k;      
} SVDResult;
SVDResult* compute_truncated_svd(Matrix* A, int k);
void free_svd_result(SVDResult* svd);
#endif 

