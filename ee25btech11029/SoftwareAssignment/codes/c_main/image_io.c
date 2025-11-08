#include "image_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PGM_BUFFER_SIZE 256
Matrix* read_pgm(const char* filename) {
    FILE* file = fopen(filename, "r");
    char buffer[PGM_BUFFER_SIZE];
    if (fgets(buffer, PGM_BUFFER_SIZE, file) == NULL) {
        fprintf(stderr, "its empy ig.\n");
        fclose(file);
        return NULL;
    }
    if (strncmp(buffer, "P2", 2) != 0) {
        fprintf(stderr, "its not my requeried magic numebr (p2).\n");
        fclose(file);
        return NULL;
    }
    do {
        if (fgets(buffer, PGM_BUFFER_SIZE, file) == NULL) {
            fprintf(stderr, "nothng is there after header.\n");
            fclose(file);
            return NULL;
        }
    } while (buffer[0] == '#');
    int width, height;
    if (sscanf(buffer, "%d %d", &width, &height) != 2) {
        fprintf(stderr, "no w or h.\n");
        fclose(file);
        return NULL;
    }
    int max_val;
    if (fscanf(file, "%d", &max_val) != 1) {
        fprintf(stderr, "no ma gray value ig.\n");
        fclose(file);
        return NULL;
    }
    Matrix* mat = create_matrix(height, width);
    int pixel_val;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (fscanf(file, "%d", &pixel_val) != 1) {
                fprintf(stderr, "shit has less data\n");
                free_matrix(mat);
                fclose(file);
                return NULL;
            }
            mat->data[i][j] = (float)pixel_val;
        }
    }
    fclose(file);
    return mat;
}
void write_pgm(const char* filename, Matrix* mat) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", mat->cols, mat->rows);
    fprintf(file, "255\n");

    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            float val = mat->data[i][j];
            if (val < 0.0)   val = 0.0;
            if (val > 255.0) val = 255.0;
            fprintf(file, "%d ", (int)val);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int read_ppm(const char* filename, Matrix* rgb_out[3]) {
    FILE* file = fopen(filename, "r");
    char buffer[PGM_BUFFER_SIZE];
    if (fgets(buffer, PGM_BUFFER_SIZE, file) == NULL) {
        fprintf(stderr, "shit its empty.\n");
        fclose(file);
        return -1;
    }
    if (strncmp(buffer, "P3", 2) != 0) {
        fprintf(stderr, "not the required magic numnebr loser.\n");
        fclose(file);
        return -1;
    }
    do {
        if (fgets(buffer, PGM_BUFFER_SIZE, file) == NULL) {
            fprintf(stderr, "nothin after headign.\n");
            fclose(file);
            return -1;
        }
    } while (buffer[0] == '#');
    int width, height;
    if (sscanf(buffer, "%d %d", &width, &height) != 2) {
        fprintf(stderr, "no w or h.\n");
        fclose(file);
        return -1;
    }
    int max_val;
    if (fscanf(file, "%d", &max_val) != 1) {
        fprintf(stderr, "no max gray vlaue.\n");
        fclose(file);
        return -1;
    }
    Matrix* R = create_matrix(height, width);
    Matrix* G = create_matrix(height, width);
    Matrix* B = create_matrix(height, width);
    int r_val, g_val, b_val;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (fscanf(file, "%d %d %d", &r_val, &g_val, &b_val) != 3) {
                fprintf(stderr, "ran out of data to put \n");
                free_matrix(R);
                free_matrix(G);
                free_matrix(B);
                fclose(file);
                return -1;
            }
            R->data[i][j] = (float)r_val;
            G->data[i][j] = (float)g_val;
            B->data[i][j] = (float)b_val;
        }
    }

    fclose(file);
    rgb_out[0] = R;
    rgb_out[1] = G;
    rgb_out[2] = B;
    return 0; 
}

//to writ back to the file 
void write_ppm(const char* filename, Matrix* R, Matrix* G, Matrix* B) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", R->cols, R->rows);
    fprintf(file, "255\n");
    for (int i = 0; i < R->rows; i++) {
        for (int j = 0; j < R->cols; j++) {
            float r_val = R->data[i][j];
            float g_val = G->data[i][j];
            float b_val = B->data[i][j];
            if (r_val < 0.0) r_val = 0.0;
            if (r_val > 255.0) r_val = 255.0;
            if (g_val < 0.0) g_val = 0.0;
            if (g_val > 255.0) g_val = 255.0;
            if (b_val < 0.0) b_val = 0.0;
            if (b_val > 255.0) b_val = 255.0;
            fprintf(file, "%d %d %d ", (int)r_val, (int)g_val, (int)b_val);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
