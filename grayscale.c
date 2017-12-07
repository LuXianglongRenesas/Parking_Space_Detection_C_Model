#include "grayscale.h"
//#include "read_bmp.h"
#include "configuration.h"

#include <stdio.h>
#include <stdlib.h>

image_matrix_rgb matrix_rgb[I_Width][I_Length];
//extern image_matrix_gray image_gray[I_Width][I_Length];
extern int image_gray[I_Width][I_Length];

void grayscale()
{

for(int j = 0; j < I_Width; j++)
    {
    	for(int i = 0 ; i < I_Length ; i++)
    	{
    		image_gray[j][i] = matrix_rgb[j][i].R * 0.2989 + matrix_rgb[j][i].G * 0.5870 + matrix_rgb[j][i].B * 0.1140;

    		//image_gray[j][i] = image_matrix_bmp[j][i].R * 0.21 + image_matrix_bmp[j][i].G * 0.72 + image_matrix_bmp[j][i].B * 0.07;

			//image_gray[j][i] = (image_matrix_bmp[j][i].R + image_matrix_bmp[j][i].G + image_matrix_bmp[j][i].B)/3;

    		//printf("gray scale pixel [%d][%d] is : %d \n",j,i, image_gray[j][i]);
    	}
    }  
	
	//printf("gray scale pixel [][] is : %f \n",i, gray_img[i]);

}
