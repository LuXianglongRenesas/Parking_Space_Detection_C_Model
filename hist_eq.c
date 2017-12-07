#include "configuration.h"
#include "hist_eq.h"
#include <stdio.h>
#include <stdlib.h>

#define FIXED_POINT_SHIFT  16

image_matrix_rgb matrix_rgb[I_Width][I_Length];
int hist_2d_arr[hist_eq_window_length][hist_eq_window_width];
int hist_1d_arr[hist_eq_window_length*hist_eq_window_width];
int hist_1d_arr_full[I_Width*I_Length];
int new_hist_1d_arr[hist_eq_window_length*hist_eq_window_width];
int new_hist_1d_arr_full[I_Width*I_Length];
int size_full = I_Width*I_Length;
int size_sliding_window = hist_eq_window_length*hist_eq_window_width;
int max_R_value;
extern int hist_eq_matrix_arr[I_Width][I_Length];

extern int hist_eq_matrix_arr_full_R[I_Width][I_Length];
extern int hist_eq_matrix_arr_full_G[I_Width][I_Length];
extern int hist_eq_matrix_arr_full_B[I_Width][I_Length];

extern image_matrix_rgb hist_eq_matrix[I_Width][I_Length]; //output of histogram_equalization function 


void histogram_equalization(char RGB_Channel)
{
//2d arr to 1d arr conversion
	for (int q = 0; q < I_Length; q++)
	{
		for (int p = 0; p < I_Width; p++)
		{
			if(RGB_Channel == 'R')
				hist_1d_arr_full[p + q*I_Width] = matrix_rgb[p][q].R; //pointer as input for function apply_histogram_equalization
			//printf("old hist_1d_arr_full[%d] value is %d \n",p + q*I_Width,hist_1d_arr_full[p + q*I_Width]);
			else if(RGB_Channel == 'G')
				hist_1d_arr_full[p + q*I_Width] = matrix_rgb[p][q].G; 
			else
				hist_1d_arr_full[p + q*I_Width] = matrix_rgb[p][q].B;
		}
	}

	apply_histogram_equalization(hist_1d_arr_full, new_hist_1d_arr_full, size_full);

	for (int q = 0; q < I_Length; q++)
	{
		for (int p = 0; p < I_Width; p++)
		{
			if(RGB_Channel == 'R')
				hist_eq_matrix_arr_full_R[p][q] = new_hist_1d_arr_full[p + q*I_Width];
			else if(RGB_Channel == 'G')
				hist_eq_matrix_arr_full_G[p][q] = new_hist_1d_arr_full[p + q*I_Width];
			else
				hist_eq_matrix_arr_full_B[p][q] = new_hist_1d_arr_full[p + q*I_Width];
		}
	}

}


void histogram_equalization_sliding_window(){
	int count_1 = 0;
	int count_2 = 0;
//applay histogram_equalization to RGB 3 channels individually
	for (int i = 0; i < num_window_hori ; i++)
	{
		for (int j = 0; j < num_window_verti ; j++)
		{ //generate temp sliding window 2d array 
			count_1 = 0;
			count_2 = 0;

			for (int m = i*hist_eq_window_length ; m < (i+1)*hist_eq_window_length ; m++)
			{
				count_2 = 0;
				for (int n = j*hist_eq_window_width ; n < (j+1)*hist_eq_window_width ; n++)
				{
					hist_2d_arr[count_1][count_2] = matrix_rgb[n][m].R;
					//printf("matrix_rgb[%d][%d] R value is %d \n", m,n,matrix_rgb[n][m].R);
					//printf("hist_2d_arr[%d][%d] vslue is %d \n",count_1,count_2,hist_2d_arr[count_1][count_2]);

					count_2++;
				}
				count_1++;
			}
		//2d arr to 1d arr conversion
		for (int q = 0; q < hist_eq_window_length; q++)
		{
			for (int p = 0; p < hist_eq_window_width; p++)
			{
				hist_1d_arr[p + q*hist_eq_window_width] = hist_2d_arr[q][p]; //pointer as input for function apply_histogram_equalization
			}
		}

	//test1
		// for (int q = 0; q < hist_eq_window_length; q++)
		// {
		// 	for (int p = 0; p < hist_eq_window_width; p++)
		// 	{
		// 		//printf("old hist_1d_arr[%d] is %d \n",p + q*hist_eq_window_width, hist_1d_arr[p + q*hist_eq_window_width]);
		// 		//hist_1d_arr[p + q*hist_eq_window_width] = hist_2d_arr[q][p]; //pointer as input for function apply_histogram_equalization
		// 	}
		// }

		apply_histogram_equalization(hist_1d_arr, new_hist_1d_arr, size_sliding_window);

		//1d arr convert back to 2d and merge them into a complete img
		for (int q = 0; q < hist_eq_window_length; q++)
		{
			for (int p = 0; p < hist_eq_window_width; p++)
			{
				hist_2d_arr[q][p] = new_hist_1d_arr[p + q*hist_eq_window_width];
			}
		}

			count_1 = 0;
			count_2 = 0;

			for (int m = i*hist_eq_window_length ; m < (i+1)*hist_eq_window_length ; m++)
			{
				count_2 = 0;
				for (int n = j*hist_eq_window_width ; n < (j+1)*hist_eq_window_width ; n++)
				{
					hist_eq_matrix[n][m].R = hist_2d_arr[count_1][count_2];
					hist_eq_matrix_arr[n][m] = hist_2d_arr[count_1][count_2];
					printf("hist_eq_matrix[%d][%d] R value is %d \n", m,n,hist_eq_matrix[n][m].R);
					//printf("hist_2d_arr[%d][%d] vslue is %d \n",count_1,count_2,hist_2d_arr[count_1][count_2]);

					count_2++;
				}
				count_1++;
			}

		//test2
		// int count_test = 0;
		// for (int q = 0; q < hist_eq_window_length; q++)
		// {
		// 	for (int p = 0; p < hist_eq_window_width; p++)
		// 	{   
		// 		// count_test++;
		// 		// if(count_test == 10)
		// 		// 	break;
		// 		//printf("max R value is %d \n",max_R_value );
		// 		printf("new hist_1d_arr[%d] is %d \n",p + q*hist_eq_window_width, new_hist_1d_arr[p + q*hist_eq_window_width]* hist_1d_arr[p + q*hist_eq_window_width]/max_R_value);
		// 		//hist_1d_arr[p + q*hist_eq_window_width] = hist_2d_arr[q][p]; //pointer as input for function apply_histogram_equalization
		// 	}
		// }

		}
	}

}
/* fixedpoint shift is used to avoid 
 * floating point operation overhead
 * histogram equalization is done in place */


//histogram_equalization approach 1, currently being used 
// int apply_histogram_equalization(int *data, int *data_out, unsigned int size)
// {


// }

//histogram_equalization approach 2, not be used for now

int apply_histogram_equalization(int *data, int *data_out, unsigned int size)
{
  unsigned int hist[256];
  unsigned int eqHist[256];
  unsigned int histIndex = 0;
  unsigned int pixIndex = 0;
  //shifted histogram value at pos i
  unsigned long Ni = 0;
  //shifted equalized histogram value at pos i
  unsigned long Kn  = 0;


  if (!data)
  {
    printf("%s invalid data pointer \n", __FUNCTION__);
    return -1;
  }

  if (size == 0)
  {
    printf("%s invalid size\n", __FUNCTION__);
    return -1;
  }

  for (histIndex = 0; histIndex < 256; histIndex++)
  {
    hist[histIndex] = 0;
  }

  /* orginal data histogram */
  for (pixIndex = 0; pixIndex < size; pixIndex++)
  {
    hist[data[pixIndex]] += 1;
  }
  
  /* generate equalized histogram */
  for (histIndex=0; histIndex<256; histIndex++)
  {
    Ni = ((unsigned long)hist[histIndex] << FIXED_POINT_SHIFT);
    Kn += 256 * (Ni/(unsigned long)size) ;
    eqHist[histIndex] = ((unsigned long)(Kn>>FIXED_POINT_SHIFT)) & 0xff ;
  }

  /* update data */
  for (pixIndex=0; pixIndex < size; pixIndex++)
  {
    data_out[pixIndex] = ((unsigned char)(eqHist[data[pixIndex]])) & 0xff;
  }

  //printf("function applied \n");
  return 0;
}