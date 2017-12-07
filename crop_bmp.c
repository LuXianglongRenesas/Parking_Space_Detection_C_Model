#include "crop_bmp.h"

void crop_bmp()
{
	int srcDataOffset = 0;
	int dstDataOffset = 0;
	cropRGB(fopen("./Pics/parking_300_200.bmp", "rb"),srcDataOffset,I_Length,I_Width,fopen("cropRGB.bmp","wb"),dstDataOffset,200,200,0,0);
}

void cropRGB( FILE *srcImg, unsigned int srcDataOffset, unsigned int sw, unsigned int sh,
	          FILE *dstImg, unsigned int dstDataOffset, unsigned int dw, unsigned int dh,
              unsigned int cropX, unsigned int cropY)
{
	unsigned int row;
    unsigned char *rowBuffer = malloc( 3 * sw );
    fseek( srcImg, srcDataOffset + 3 * ( row * sw ), SEEK_SET );
    if( dstDataOffset > 0 ){
        fseek( dstImg, dstDataOffset, SEEK_SET );
    }
    for( row = cropY; row < cropY + dh; ++row )
    {
        fread ( rowBuffer, 1, 3 * sw, srcImg );
        fwrite( rowBuffer + 3 * cropX, 1, 3 * dw, dstImg );
    }
    free( rowBuffer );

}