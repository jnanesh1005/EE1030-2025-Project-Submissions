#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <jpeglib.h> 
// direct fucntion to skip all comments 
void skip_comments_and_whitespace(FILE *fp) {
    int ch;
    while((ch = fgetc(fp))!= EOF){
        if (ch == '#') {
            while ((ch = fgetc(fp)) != EOF && ch != '\n' && ch != '\r') {
            }
        } else if (ch == ' '||ch=='\f'||ch=='\n'||ch=='\r'||ch=='\t'||ch=='\v'){
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
    if ((infile = fopen(in_filename, "r")) == NULL) {
        fprintf(stderr, "cannot open the file for reading.\n");
        return 1;
    }
    FILE *outfile;
    if ((outfile = fopen(out_filename, "wb")) == NULL) {
        fprintf(stderr, "cannot open the file for writing\n");
        fclose(infile);
        return 1;
    }
    char magic[3];
    int width, height, max_val;

    if (fscanf(infile, "%2s", magic) != 1 || strcmp(magic, "P3") != 0) {
        fprintf(stderr, "deosnt have the magic number i want \n");
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
        fprintf(stderr, "no max color value.\n");
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    if (max_val != 255) {
        fprintf(stderr, "the quality might dec.\n", max_val);
    }
    
    fgetc(infile); 

    fprintf(stdout, "Input PPM: %dpx wide, %dpx high.\n", width, height);

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;       
    cinfo.in_color_space = JCS_RGB; 
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    int row_stride = width * 3; 
    unsigned char *row_buffer = (unsigned char *)malloc(row_stride);
    JSAMPROW row_pointer[1];
    row_pointer[0] = row_buffer;
    while (cinfo.next_scanline < cinfo.image_height) {
        for (int i = 0; i < row_stride; i++) {
            int pixel_val;
            if (fscanf(infile, "%d", &pixel_val) != 1) {
                fprintf(stderr, "ran out of data %d\n", cinfo.next_scanline);
                free(row_buffer);
                jpeg_destroy_compress(&cinfo);
                fclose(infile);
                fclose(outfile);
                return 1;
            }
            row_buffer[i] = (unsigned char)pixel_val;
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
