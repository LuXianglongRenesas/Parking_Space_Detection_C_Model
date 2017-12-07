#include "yuv_color_filter.h"
#include "configuration.h"

#include <stdio.h>
//RGB2GSV convertion and white color filtering

//initialization of self defined struct including bmp pixels
image_matrix image_matrix_bmp[I_Width][I_Length];

int hist_eq_matrix_arr_full_R[I_Width][I_Length];
int hist_eq_matrix_arr_full_G[I_Width][I_Length];
int hist_eq_matrix_arr_full_B[I_Width][I_Length];

//image_matrix_wt image_wt_filt[I_Width][I_Length];
extern int image_wt_logic[I_Width][I_Length];

int avg_Y;
//extern int image_wt_filt[I_Width][I_Length];  

void wt_color_filter()
{
	double max, min;

	for(int j = 0; j < I_Width; j++)
    {
    	for(int i = 0 ; i < I_Length ; i++)
    	{	
    		
    		//white color
    		if (hist_eq_matrix_arr_full_R[j][i] > color_filter_thres && hist_eq_matrix_arr_full_G[j][i] > color_filter_thres && hist_eq_matrix_arr_full_B[j][i] > color_filter_thres)
			//if (image_matrix_bmp[j][i].Y > avg_Y*lighting_scale_factor)
			//if (image_matrix_bmp[j][i].Y > 155)
			{
				image_wt_logic[j][i] = 255;

			}
			else {image_wt_logic[j][i] = 0;}

			//yellow color
			// if (image_matrix_bmp[j][i].U < 110 && image_matrix_bmp[j][i].V > 138)
			// {
			// 	image_wt_logic[j][i] = 255;

			// }
			// else {image_wt_logic[j][i] = 0;}
			

			//printf("HSV logic image [%d][%d] is [%d]: \n",j,i, image_wt_logic[j][i]);
			

			// H is in degrees and S V are two number between 0 and 1


			//printf("white color filtered pixel HSV[%d][%d] is [%f %f %f]: \n",j,i, image_wt_filt[j][i].H, image_wt_filt[j][i].S, image_wt_filt[j][i].V);


    	}

    }
}