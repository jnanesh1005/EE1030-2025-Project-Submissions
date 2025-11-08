#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "matrixutils.h"
#include "image_io.h"
#include "svd.h"
#include "svd_io.h"
#include "analysis.h"
int main() {
    srand(time(NULL));
    const char* image_files[] = {
        "greyscale.pgm"
    };
    int num_images = 1;
    int k_values[] = {5, 20, 50, 100};
    int num_k_values = sizeof(k_values) / sizeof(k_values[0]);
    for (int i = 0; i < num_images; i++) {
        const char* img_filename = image_files[i];
        if (strstr(img_filename, ".pgm")) {
            Matrix* A_orig = read_pgm(img_filename);
            for (int j = 0; j < num_k_values; j++) {
                int k = k_values[j];
                if (k > A_orig->cols || k > A_orig->rows) {
                    printf("Skipping k=%d (its too big ig in dim).\n", k);
                    continue;
                }
                printf("  Running for k = %d\n", k);
                SVDResult* svd = compute_truncated_svd(A_orig, k);
                if (svd == NULL) {
                    fprintf(stderr, "it failed for %d.\n", k);
                    continue;
                }
                char svd_filename[256];
                sprintf(svd_filename, "compressed_%s_k%d.svd", img_filename, k);
                write_compressed_svd(svd_filename, svd);
                free_svd_result(svd);
                svd = NULL; 

                SVDResult* loaded_svd = read_compressed_svd(svd_filename);
                Matrix* A_k = reconstruct_image(loaded_svd);
                float error = compute_frobenius_error(A_orig, A_k);
                printf("Image: %s, k=%d, Error (Frobenius): %f\n", img_filename, k, error);
                char pgm_out_filename[256];
                sprintf(pgm_out_filename, "reconstructed_%s_k%d.pgm", img_filename, k);
                write_pgm(pgm_out_filename, A_k);
                if (j == num_k_values - 1) { 
                    char s_val_filename[256];
                    sprintf(s_val_filename, "singular_values_%s.txt", img_filename);
                    write_singular_values(s_val_filename, loaded_svd);
                }

                free_matrix(A_k);
                free_svd_result(loaded_svd);
            }
            free_matrix(A_orig);

        } else if (strstr(img_filename, ".ppm")) {
            
            Matrix* rgb_orig[3];
            if (read_ppm(img_filename, rgb_orig) != 0) {
                fprintf(stderr, "shit coundnt read image %s. Skipping.\n", img_filename);
                continue;
            }
            Matrix* R_orig = rgb_orig[0];
            Matrix* G_orig = rgb_orig[1];
            Matrix* B_orig = rgb_orig[2];
            for (int j = 0; j < num_k_values; j++) {
                int k = k_values[j];
                if (k > R_orig->cols || k > R_orig->rows) {
                    printf("Skipping k=%d (its big ig).\n", k);
                    continue;
                }
                printf("Running for k = %d\n", k);
                SVDResult* svd_R = compute_truncated_svd(R_orig, k);
                SVDResult* svd_G = compute_truncated_svd(G_orig, k);
                SVDResult* svd_B = compute_truncated_svd(B_orig, k);
                char s_val_filename[256];
                sprintf(s_val_filename, "singular_values_%s_R.txt", img_filename);
                write_singular_values(s_val_filename, svd_R);

                Matrix* R_k = reconstruct_image(svd_R);
                Matrix* G_k = reconstruct_image(svd_G);
                Matrix* B_k = reconstruct_image(svd_B);

                if(R_k == NULL || G_k == NULL || B_k == NULL) {
                    fprintf(stderr, "no it falied for this k=%d.\n", k);
                } else {
                    char ppm_out_filename[256];
                    sprintf(ppm_out_filename, "reconstructed_%s_k%d.ppm", img_filename, k);
                    write_ppm(ppm_out_filename, R_k, G_k, B_k);
                }
                free_matrix(R_k);
                free_matrix(G_k);
                free_matrix(B_k);
                free_svd_result(svd_R);
                free_svd_result(svd_G);
                free_svd_result(svd_B);
            }

            free_matrix(R_orig);
            free_matrix(G_orig);
            free_matrix(B_orig);
        }
    }

    printf("done with the code\n");
    return 0;
}
