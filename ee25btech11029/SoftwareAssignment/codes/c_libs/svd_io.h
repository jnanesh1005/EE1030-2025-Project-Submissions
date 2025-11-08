#ifndef SVD_IO_H
#define SVD_IO_H

#include "svd.h" 
void write_compressed_svd(const char* filename, SVDResult* svd);
SVDResult* read_compressed_svd(const char* filename);

#endif

