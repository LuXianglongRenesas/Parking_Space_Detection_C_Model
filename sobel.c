#include <stdio.h>
#include <stdlib.h>
#include <float.h>
//#include "grayscale.h"
#include <math.h>
#include "configuration.h"

int image_wt_logic[I_Width][I_Length];

//
int image_gray[I_Width][I_Length];
extern int img_sobel[I_Width][I_Length];
extern int img_sobel_visual[I_Width][I_Length];
int y_size1 = I_Width, x_size1 = I_Length, y_size2 = I_Width, x_size2 = I_Length;
int MAX_BRIGHTNESS = 255;

void sobel_filtering( )
     /* Spatial filtering of image data */
     /* Sobel filter (horizontal differentiation */
     /* Input: image_gray[y][x] ---- Outout: img_sobel[y][x] */
{
  /* Definition of Sobel filter in horizontal direction */
  int weight_hori[3][3] = {{ -1,  0,  1 },
		      { -2,  0,  2 },
		      { -1,  0,  1 }};
  int weight_vert[3][3] = {{-1, -2, -1},
          {  0,  0,  0 },
          {  1,  2,  1 }};

  double pixel_value,pixel_value_visual,pixel_value_1, pixel_value_2;

  double min, max;
  int x, y, i, j;  /* Loop variable */
  
  /* Maximum values calculation after filtering*/
  printf("Now, filtering of input image is performed\n\n");
  min = DBL_MAX;
  max = -DBL_MAX;
  for (y = 1; y < y_size1 - 1; y++) {
    for (x = 1; x < x_size1 - 1; x++) {
      pixel_value = 0.0;
      pixel_value_1 = 0.0;
      pixel_value_2 = 0.0;
      for (j = -1; j <= 1; j++) {
	    for (i = -1; i <= 1; i++) {
	      pixel_value_1 += weight_hori[j + 1][i + 1] * image_wt_logic[y + j][x + i];
        pixel_value_2 += weight_vert[j + 1][i + 1] * image_wt_logic[y + j][x + i];
        pixel_value = sqrt(pixel_value_1*pixel_value_1 + pixel_value_2*pixel_value_2) ;
	    }
      }
      if (pixel_value < min) min = pixel_value;
      if (pixel_value > max) max = pixel_value;
    }
  }
  if ((int)(max - min) == 0) {
    printf("Nothing exists!!!\n\n");
    exit(1);
  }

  /* Initialization of img_sobel[y][x] */
  x_size2 = x_size1;
  y_size2 = y_size1;
  for (y = 0; y < y_size2; y++) {
    for (x = 0; x < x_size2; x++) {
      img_sobel[y][x] = 0;
    }
  }
  /* Generation of img_sobel after linear transformtion */
  for (y = 1; y < y_size1 - 1; y++) {
    for (x = 1; x < x_size1 - 1; x++) {
      pixel_value = 0.0;
      pixel_value_1 = 0.0;
      pixel_value_2 = 0.0;

      for (j = -1; j <= 1; j++) {
	    for (i = -1; i <= 1; i++) {
	      pixel_value_1 += weight_hori[j + 1][i + 1] * image_wt_logic[y + j][x + i];
        pixel_value_2 += weight_vert[j + 1][i + 1] * image_wt_logic[y + j][x + i];
        pixel_value = sqrt(pixel_value_1*pixel_value_1 + pixel_value_2*pixel_value_2) ;
	    }
      }
      pixel_value = MAX_BRIGHTNESS * (pixel_value - min) / (max - min);

      if(pixel_value != 0) 
        {pixel_value = 1;
        pixel_value_visual = 255;
        }
      else {pixel_value = 0;
            pixel_value_visual = 0;
            }
      
      img_sobel[y][x] = (unsigned char)pixel_value;
      img_sobel_visual[y][x] = (unsigned char)pixel_value_visual;
      //printf("sobel filtering pixel [%d][%d] is : %d \n",y,x, img_sobel[y][x]);
    }
  }
}
