# SVD Image Compression in C

This project is a from-scratch implementation of image compression using Truncated Singular Value Decomposition (SVD), written entirely in C. The algorithm uses the Power Iteration method with Hotelling's deflation to compute the SVD components.

The project is managed by a Python helper script (`convert.py`) which orchestrates the entire process: running the C code, conducting post-analysis, and performing comparisons.

## Project Structure

The C program is split into several modules for clarity and organization:

- **matrixutils.c.h**  
  The foundation. Handles all matrix and vector math, creation, and memory management.
- **image_io.c.h**  
  The translator. Handles reading/writing PGM (grayscale) and PPM (color) image files.
- **svd.c.h**  
  The brain. Implements the efficient SVD algorithm using Power Iteration and deflation.
- **svd_io.c.h**  
  The compressor. Implements the custom `.svd` file format for saving/loading the compressed components (U, S, V).
- **analysis.c.h**  
  The rebuilder. Reconstructs the image from SVD components and calculates the Frobenius norm error.
- **main.c**  
  The conductor. Orchestrates the C program's logic, processing images through the full compress/decompress pipeline.
- **convert.c**  
  Converts JPEG to PGM.
- **pgm_to_jpeg.c**  
  Converts PGM to JPEG.
- **ppm_to_jpeg.c**  
  Converts PPM to JPEG.
- **convert.py**  
  The main Python script. This is the only file you need to run. It executes the C program and runs bonus analysis (plotting, NumPy comparison).

