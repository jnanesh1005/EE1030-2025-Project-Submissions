
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h> 
int main(int argc, char *argv[]) {
    char *in_filename = argv[1];
    char *out_filename = argv[2];
    FILE *infile;
    if ((infile = fopen(in_filename, "rb")) == NULL) {
        fprintf(stderr, "coudnt open it for reading\n");
        return 1;
    }
    FILE *outfile;
    if ((outfile = fopen(out_filename, "w")) == NULL) {
        fprintf(stderr, "cannot open %s for writing.\n", out_filename);
        fclose(infile);
        return 1;
    }
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void)jpeg_read_header(&cinfo, TRUE);
    (void)jpeg_start_decompress(&cinfo);
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int num_components = cinfo.output_components; 
    fprintf(stdout, "Input image: %dpx wide, %dpx high, %d components.\n", width, height, num_components);
    if (num_components != 1 && num_components != 3) {
        fprintf(stderr, "it was not supported.\n");//this is if i use color by mistake
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    
    
    
    
    fprintf(outfile, "P2\n");
    fprintf(outfile, "%d %d\n", width, height);
    fprintf(outfile, "255\n");

    
    
    int row_stride = width * num_components;
    
    
    
    unsigned char *buffer = (unsigned char *)malloc(row_stride);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for scanline buffer.\n");
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    
    while (cinfo.output_scanline < cinfo.output_height) {
        
        
        
        
        (void)jpeg_read_scanlines(&cinfo, &buffer, 1);

        
        for (int i = 0; i < width; i++) {
            unsigned char gray_value;

            if (num_components == 3) {
                
                
                unsigned char r = buffer[i * 3];
                unsigned char g = buffer[i * 3 + 1];
                unsigned char b = buffer[i * 3 + 2];
                
                
                gray_value = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            } else {
                
                gray_value = buffer[i];
            }

            
            fprintf(outfile, "%d ", gray_value);
        }
        
        
        fprintf(outfile, "\n");
    }

    
    
    free(buffer);

    
    (void)jpeg_finish_decompress(&cinfo);

    
    jpeg_destroy_decompress(&cinfo);

    
    fclose(infile);
    fclose(outfile);

    fprintf(stdout, "Successfully converted %s to %s.\n", in_filename, out_filename);

    return 0;
}
