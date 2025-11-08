#ifndef IMAGE_IO_H
#define IMAGE_IO_H
#include "matrixutils.h"
Matrix* read_pgm(const char* filename);
void write_pgm(const char* filename, Matrix* mat);
int read_ppm(const char* filename, Matrix* rgb_out[3]);
void write_ppm(const char* filename, Matrix* R, Matrix* G, Matrix* B);
#endif
