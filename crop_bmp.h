#ifndef _CROP_BMP_H_
#define _CROP_BMP_H_
#include <stdio.h>
#include <stdlib.h>
#include "read_bmp.h"

void crop_bmp();

void cropRGB( FILE *srcImg, unsigned int srcDataOffset, unsigned int sw, unsigned int sh,
              FILE *dstImg, unsigned int dstDataOffset, unsigned int dw, unsigned int dh,
              unsigned int cropX, unsigned int cropY);

#endif