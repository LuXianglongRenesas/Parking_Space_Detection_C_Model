//C code for parking spot detection algorithm testing
//Author : Xianglong Lu
//Company: Renesas Electronics America
// July 2017
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "configuration.h"
//#include "grayscale.h"
#include "writeBMP.h"
#include "sobel.h"
#include "radon.h"
#include "yuv_color_filter.h"
#include "spline_fitting.h"
#include "parking_space_detection.h"
#include "bmp_crop.h"
#include "grayscale.h"
#include "hist_eq.h"
//#include <gtk/gtk.h>
//#include <cairo.h>
//#include <graphics.h>
//#include <conio.h>
image_matrix image_matrix_bmp[I_Width][I_Length];
image_matrix_rgb matrix_rgb[I_Width][I_Length];
image_matrix_hsv matrix_hsv[I_Width][I_Length];
image_matrix_rgb hist_eq_matrix[I_Width][I_Length];
int hist_eq_matrix_arr[I_Width][I_Length];

int hist_eq_matrix_arr_full_R[I_Width][I_Length];
int hist_eq_matrix_arr_full_G[I_Width][I_Length];
int hist_eq_matrix_arr_full_B[I_Width][I_Length];
//image_matrix_wt image_wt_filt[I_Width][I_Length];

int image_gray[I_Width][I_Length];
int img_sobel[I_Width][I_Length];
int img_sobel_visual[I_Width][I_Length];
//int image_wt_filt[I_Width][I_Length];  
int image_wt_logic[I_Width][I_Length];
double P_2d[Radon_rSize][radon_angle_range];
int P_2d_int[I_Length][radon_angle_range];
int num_marker;
int perp_flag;

extern int rSize;

double THETA[radon_angle_range];
// double THETA_PERP[radon_angle_range];
// double THETA_PRAL[radon_angle_range];
int main(int argc, char *argv[])
{ 

//timer
clock_t launch = clock();
//main processing functions 
printf("STEP 1 - START READING BMP IMAGE \n\n");

rSize = read_bmp(); //return verticle size of radon space 
//printf("Data Received , RGB value of pixel [%d,%d] is [%d %d %d] \n", 300 , 300\
 , image_matrix_bmp[300][300].R, image_matrix_bmp[300][300].G, image_matrix_bmp[300][300].B);
printf("rSize value (verticle length of RADON transform) is : %d \n\n", rSize);
//[OPTION 1]
printf("STEP 2 - START GRAY SCALE CONVERTION PROCESS \n\n");

//grayscale();  	//call gray scale funtion

printf("STEP 3 - START HISTOGRAM EQUALIZATION \n\n");

histogram_equalization('R');
histogram_equalization('G');
histogram_equalization('B');

//printf("gray image 500th pixel is %f",gray_img[1000]);
printf("STEP 4 - START COLOR FILTERING PREOCESS \n\n");
// put white color filtering here , so gray scale can be optional 

wt_color_filter(); //white color filtering , for white lane marker detection only

//spline_fitting();
printf("STEP 5 - START SOBEL EDGE DETECTION PREOCESS \n\n");

//sobel_filtering(); // put white color filtering here , so gray scale can be optional 

//writeBMP(hist_eq_matrix_arr_full_B, I_Width, I_Length); 
//writeBMP(image_wt_logic, I_Width, I_Length); 

//writeBMP(img_sobel_visual, I_Width, I_Length); 

//you may change range of theta selection

for (int i = 0; i < randon_angle_offset ; i++){
	THETA[i] = i + (90-randon_angle_offset/2) - 90;
    }

//clock_t launch_radon = clock();

//radon_transform(img_sobel, &THETA_PERP[0]);

//clock_t done_radon = clock();

for (int j = randon_angle_offset; j < radon_angle_range ; j++){
	THETA[j] = j - randon_angle_offset - randon_angle_offset/2 - 90;
    }

 clock_t launch_radon = clock();

 radon_transform(image_wt_logic, &THETA[0]);

 clock_t done_radon = clock();


// for (int i = randon_angle_offset; i < radon_angle_range ; i++){
// 	THETA[i] = i - randon_angle_offset + (90-randon_angle_offset/2) - 90;
//     }


// for (int i = 0; i < randon_angle_offset ; i++){
// 	THETA[i] = i -randon_angle_offset/2 - 90;
//     }



// for (int i = 0; i < radon_angle_range ; i++){
// 	THETA[i] = i - 90;
//     }

printf("STEP 5 - START RADON TRANSFORM PREOCESS \n\n");


// clock_t launch_radon = clock();

// radon_transform(img_sobel, &THETA[0]);

// clock_t done_radon = clock();

// for (int i = 0; i < 180; i++)
//     {
//         for (int j = 0; j < 180; j++)
//         { 
// 		P_2d_int[i][j] = (int) P_2d[i][j];
//         }
//     }

//writeBMP(P_2d_int, 900, 180); 
//perp_flag = peak_point_filter();
printf("START PARKING SPACE DETECTION \n \n");

parking_space_detection();

bmp_crop();

//UDP_client();
clock_t done = clock();
double diff = (double)(done - launch) / CLOCKS_PER_SEC;
double diff_radon = (double)(done_radon - launch_radon) / CLOCKS_PER_SEC;

printf("\nTotal time is : %f seconds \n\n", diff);
printf("Total time of radon transform is : %f seconds \n\n", diff_radon);


return 0;

}
