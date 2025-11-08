#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "matrixutils.h"
#include "svd.h"
#include <stdio.h>
struct Matrix* reconstruct_image(struct SVDResult* svd);
float compute_frobenius_error(struct Matrix* A, struct Matrix* A_k);
void write_singular_values(const char* filename, SVDResult* svd);
#endif  