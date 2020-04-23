/* LoadPNG.cpp */
/* Code for reading and writing png files */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include "image.h"

void GetPNGDimensions(const char *filename, int &w, int &h)
{
    FILE *f;
    
    if ((f = fopen(filename, "rb")) == NULL) {
        printf("[GetPNGDimensions] Error: can't open file %s for reading\n", 
               filename);
        return;
    }

    png_byte sig[8];
    fread(sig, 1, 8, f);
    if (png_sig_cmp(sig, 0, 8))
        printf("[GetPNGDimensions] File %s is not recognized as a PNG file\n", 
               filename);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) abort();
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) abort();

    if(setjmp(png_jmpbuf(png_ptr))) abort();
    
    png_set_sig_bytes(png_ptr, 8);
    png_init_io(png_ptr, f);
    png_read_info(png_ptr, info_ptr);

    w = png_get_image_width(png_ptr, info_ptr);
    h = png_get_image_height(png_ptr, info_ptr);
    
    printf("[GetPNGDimensions] File %s: ( %d , %d )\n", filename, w, h);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(f);
}

img_t *LoadPNG(const char *filename)
{
    FILE *f;
    
    if ((f = fopen(filename, "rb")) == NULL) {
        printf("[GetPNGDimensions] Error: can't open file %s for reading\n", 
               filename);
        return NULL;
    }

    png_byte sig[8];
    fread(sig, 1, 8, f);
    if (png_sig_cmp(sig, 0, 8))
        printf("[GetPNGDimensions] File %s is not recognized as a PNG file\n", 
               filename);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) abort();
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) abort();

    if(setjmp(png_jmpbuf(png_ptr))) abort();
    
    png_set_sig_bytes(png_ptr, 8);
    png_init_io(png_ptr, f);
    png_read_info(png_ptr, info_ptr);

    int w = png_get_image_width(png_ptr, info_ptr);
    int h = png_get_image_height(png_ptr, info_ptr);
    int n = png_get_channels(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    assert(color_type == PNG_COLOR_TYPE_RGB && bit_depth == 8 && (n == 1 || n == 3));
    printf("[GetPNGDimensions] File %s: ( %d , %d, %d )\n", filename, w, h, n);

    img_t *img = img_new(w, h);
    
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * h);
    for(int y = 0; y < h; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
    }
    png_read_image(png_ptr, row_pointers);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (n == 3) {
                img_set_pixel(img, x, h - y - 1, 
                              row_pointers[y][3 * x + 0], row_pointers[y][3 * x + 1], row_pointers[y][3 * x + 2]);
            } else if (n == 1) {
                ;
                img_set_pixel(img, x, h - y - 1, row_pointers[y][x], row_pointers[y][x], row_pointers[y][x]);
            }
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(f);

    return img;
}
