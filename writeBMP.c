#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "writeBMP.h"

void writeBMP(int Matrix[I_Width][I_Length], uint32_t height, uint32_t width){

    FILE *out;
    int ii,jj;
    long pos = 1077;
    //long pos = 1100;
 
 
    out = fopen("output.bmp","wb");
 
 
    // Image Signature
    unsigned char signature[2] = {'B','M'};
    fseek(out,0,0);
    fwrite(&signature,2,1,out);
 
 
    // Image file size
    uint32_t filesize = 54 + 4*256 + height*width;
    fseek(out,2,0);
    fwrite(&filesize,4,1,out);
 
 
    // Reserved
    uint32_t reserved = 0;
    fseek(out,6,0);
    fwrite(&reserved,4,1,out);
 
 
    // Offset
    uint32_t offset = 1078;
    fseek(out,10,0);
    fwrite(&offset,4,1,out);
 
 
   // Info header size
    uint32_t ihsize = 40;
    fseek(out,14,0);
    fwrite(&ihsize,4,1,out);
 
 
    // Image Width in pixels
    //uint32_t width = (uint32_t) width;
    fseek(out,18,0);
    fwrite(&width,4,1,out);
 
 
    // Image Height in pixels
    //uint32_t height = (uint32_t) height;
    fseek(out,22,0);
    fwrite(&height,4,1,out);
 
 
    // Number of planes
    uint16_t planes = 1;
    fseek(out,26,0);
    fwrite(&planes,2,1,out);
 
 
    // Color depth, BPP (bits per pixel)
    uint16_t bpp = 8;
    fseek(out,28,0);
    fwrite(&bpp,2,1,out);
 
 
    // Compression type
    uint32_t compression = 0;
    fseek(out,30,0);
    fwrite(&compression,4,1,out);
 
 
    // Image size in bytes
    uint32_t imagesize = (uint32_t) height*width;
    fseek(out,34,0);
    fwrite(&imagesize,4,1,out);
 
 
    // Xppm
    uint32_t xppm = 0;
    fseek(out,38,0);
    fwrite(&xppm,4,1,out);
 
 
    // Yppm
    uint32_t yppm = 0;
    fseek(out,42,0);
    fwrite(&yppm,4,1,out);
 
 
    // Number of color used (NCL)
    uint32_t colours = 256;
    fseek(out,46,0);
    fwrite(&colours,4,1,out);
 
 
    // Number of important color (NIC)
    // value = 0 means all collors important
    uint32_t impcolours = 0;
    fseek(out,50,0);
    fwrite(&impcolours,4,1,out);
 
 
    // Colour table
    unsigned char bmpcolourtable[1024];
    for(ii=0; ii < 1024; ii++){
        bmpcolourtable[ii] =  0;
    }
    jj=3;
    for(ii=0; ii < 255; ii++){
        bmpcolourtable[jj+1] =  ii+1;
        bmpcolourtable[jj+2] =  ii+1;
        bmpcolourtable[jj+3] =  ii+1;
        jj=jj+4;
    }
 
 
    fseek(out,54,0);
    fwrite(&bmpcolourtable,256,4,out);
 
 
    for(ii=0;ii<height;ii++){
        for(jj=0;jj<width;jj++){
            pos+= 1;
            fseek(out,pos,0);
            fwrite(&Matrix[ii][jj],(sizeof(unsigned char)),1,out);
        }
    }
 
 
    fflush(out);
    fclose(out);
}