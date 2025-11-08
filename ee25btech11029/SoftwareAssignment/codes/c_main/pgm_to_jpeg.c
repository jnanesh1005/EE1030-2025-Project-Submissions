#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <jpeglib.h> 
void skip_comments_and_whitespace(FILE *fp) {
    int ch;
    while ((ch=fgetc(fp))!=EOF) {
        if (ch=='#') {
            while((ch=fgetc(fp))!=EOF&&ch!='\n'&&ch!='\r'){
            }
        } else if (ch==' '||ch=='\f'||ch=='\n'||ch=='\r'||ch=='\t'||ch=='\v'){
            continue;
        } else {
            ungetc(ch, fp);
            return;
        }
    }
}


int main(int argc, char *argv[]) {
    char *in_filename = argv[1];
    char *out_filename = argv[2];
    FILE *infile;
    if ((infile = fopen(in_filename, "rb")) == NULL) {
        fprintf(stderr, "cannot read %s.\n", in_filename);
        return 1;
    }
    FILE *outfile;
    if ((outfile = fopen(out_filename, "wb")) == NULL) {
        fprintf(stderr, "cannot write %s.\n", out_filename);
        fclose(infile);
        return 1;
    }
    char magic[3];
    int width, height, max_val;
    int is_ascii;
    if (fscanf(infile, "%2s", magic) != 1) {
        fprintf(stderr, "Could not read PGM magic number.\n");
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    if (strcmp(magic, "P2") == 0) {
        is_ascii = 1;
    } else if (strcmp(magic, "P5") == 0) {
        is_ascii = 0;
    } else {
        fprintf(stderr, "does not have teh magic number i am looking for. Magic: %s\n", magic);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    skip_comments_and_whitespace(infile);
    if (fscanf(infile, "%d", &width) != 1) {
        fprintf(stderr, "no w.\n");
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    skip_comments_and_whitespace(infile);
    if (fscanf(infile, "%d", &height) != 1) {
        fprintf(stderr, "no h.\n");
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    skip_comments_and_whitespace(infile);
    if (fscanf(infile, "%d", &max_val) != 1) {
        fprintf(stderr, "no max color vlaue.\n");
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    if (max_val != 255) {
        fprintf(stderr, "migth decrease the quality even more.\n", max_val);
    }
    fgetc(infile); 
    fprintf(stdout, "Input PGM (%s): %dpx wide, %dpx high.\n", magic, width, height);
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 1;         
    cinfo.in_color_space = JCS_GRAYSCALE; 
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    int row_stride = width * 1; 
    unsigned char *row_buffer = (unsigned char *)malloc(row_stride);
    JSAMPROW row_pointer[1]; 
    row_pointer[0] = row_buffer;
    while (cinfo.next_scanline < cinfo.image_height) {
        if (is_ascii) {
            for (int i = 0; i < width; i++) {
                int pixel_val;
                if (fscanf(infile, "%d", &pixel_val) != 1) {
                    fprintf(stderr, "ran out of data to add %d\n", cinfo.next_scanline);
                    free(row_buffer);
                    jpeg_destroy_compress(&cinfo);
                    fclose(infile);
                    fclose(outfile);
                    return 1;
                }
                row_buffer[i] = (unsigned char)pixel_val;
            }
        } else {
            size_t bytes_read = fread(row_buffer, 1, row_stride, infile);
            if (bytes_read != row_stride) {
                fprintf(stderr, "ran ou tof pixel data %d. Expected %d, got %zu\n",
                        cinfo.next_scanline, row_stride, bytes_read);
                free(row_buffer);
                jpeg_destroy_compress(&cinfo);
                fclose(infile);
                fclose(outfile);
                return 1;
            }
        }
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    free(row_buffer);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(infile);
    fclose(outfile);
    fprintf(stdout, "Successfully converted %s to %s.\n", in_filename, out_filename);
    return 0;
}
