/* LoadPNG.h */
/* Code for reading and writing png files */

#ifndef __load_png_h__
#define __load_png_h__

#include "image.h"

img_t *LoadPNG(const char *filename);
void GetPNGDimensions(const char *filename, int &w, int &h);
// void WritePNG(const img_t *img, const char *filename);

#endif /* __load_png_h__ */
