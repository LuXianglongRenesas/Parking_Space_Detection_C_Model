#include "configuration.h"
#include "spline_fitting.h"
#include <math.h>
#include <stdio.h>
#include "polifitgsl.h"

#define num_window 20
#define DEGREE 3
//parameter initialization
int img_width = I_Width;
int img_length = I_Length;
int slid_window_width = I_Width/4;
int slid_window_length = I_Length/num_window;
int init_box_width = I_Width;
int init_box_length = I_Length/num_window;

int image_wt_logic[I_Width][I_Length];
int sample_matrix[num_window][2];
double poly_matrix[num_window][2];
double poly_matrix_x[num_window];
double poly_matrix_y[num_window];
double coeff[DEGREE];

void spline_fitting(){
	//loop parameters
	int count = 0;
	int hist_peak = 0;
	int peak_index = 0;
	int sum = 0;
	int win_index = 0;
	int sum_pixel_x = 0;
	int sum_pixel_y = 0;
	int new_start_pt = 0;
	double scalling_factor_width = 3.0/img_width;
	double scalling_factor_length = 6.0/img_length;
	//int i = 0

	// Method two
// 	//Initialize a box on the right

for ( int i = 1; i < (num_window); i++){
    for (int j = 1; j < img_width; j++){
        for (int k = img_length - i*init_box_length; k < img_length - (i-1)*init_box_length; k++){
           if(image_wt_logic[j][k] == 1){
            sum = sum + j;
            count = count + 1;
           }
        }
    
        
    }
    //printf("count value is %d \n", count);
    if(count > 1000){
    //peak_index = img_width - round(sum/count);
    peak_index = round(sum/count);
    win_index = i;
    break;
    }
    if(i == num_window){
        //peak_index = NAN;
        win_index = i;
    }
    //count = 0;
    //continue;
    
}

printf("spline fitting is performed and peak_index is %d and win_index is %d: \n" , peak_index , win_index); //379 is correct
	
// getting sample points for each box
for(int i = win_index ; i <= num_window; i++){

if(win_index == num_window){
    break;
}

if(i == win_index){
	sum = 0;
	count = 0;
	for(int j = peak_index - slid_window_width/2; j < peak_index + slid_window_width/2 ; j++){
    for(int k = img_length - i*slid_window_length; k < img_length - (i-1)*slid_window_length ; k++){
        if(image_wt_logic[j][k] == 1){
            sum = sum + j;
            count = count + 1;
            sum_pixel_x = j + sum_pixel_x;
            sum_pixel_y = k + sum_pixel_y;
        }
    }
	//printf("sum is %d and count is %d \n", sum, count);
	// new_start_pt = round(sum/count);
 //    sample_matrix[i][1] = round(sum_pixel_x / count);
 //    sample_matrix[i][2] = round(sum_pixel_y / count);
    //printf("new_start_pt is: %d \n", new_start_pt);
	}
	printf("sum is %d and count is %d \n", sum, count);
	new_start_pt = round(sum/count);
 	sample_matrix[i][1] = round(sum_pixel_x / count);
 	sample_matrix[i][2] = round(sum_pixel_y / count);

	printf("new_start_pt is: %d \n", new_start_pt);
}


if(i != win_index){

	sum = 0;
	count = 0;
	sum_pixel_x = 0;
	sum_pixel_y = 0;

	for (int j = new_start_pt - slid_window_width/2 ; j < new_start_pt + slid_window_width/2 ; j++){
    for (int k = img_length - i*slid_window_length ; k < img_length - (i-1)*slid_window_length ; k++){
        if(image_wt_logic[j][k] == 1){
            sum = sum + j;
            count = count + 1;
            sum_pixel_x = j + sum_pixel_x;
            sum_pixel_y = k + sum_pixel_y;
        }
    }

}
	if(count != 0){
	new_start_pt = round(sum/count);
    sample_matrix[i][1] = round(sum_pixel_x / count);
    sample_matrix[i][2] = round(sum_pixel_y / count);
    //printf("new start point is: %d \n", new_start_pt);
    //printf("sample_matrix no.%d is: [%d][%d] \n", i,sample_matrix[i][1],sample_matrix[i][2]); //checked correct

	}
}
}
// Polynomial Spline Fitting
// converting sample_matrix from pixel coordinate to relative coordinate in meters
// recalculate elements in matrix
for (int i = 1 ; i <= num_window; i++){

	poly_matrix[i][1] = -sample_matrix[i][1]*scalling_factor_width;
	poly_matrix[i][2] = (I_Length/2 - sample_matrix[i][2])*scalling_factor_length;
	poly_matrix_x[i] = poly_matrix[i][2];
	poly_matrix_y[i] = poly_matrix[i][1];

	//printf("poly_matrix no.%d is: [%f][%f] \n", i,poly_matrix[i][1],poly_matrix[i][2]);
	//printf("factor is : %f \n", scalling_factor_length);
}

//Polynomial Fitting starts here

// P = POLYFIT(X,Y,N) finds the coefficients of a polynomial P(X) of
//    degree N that fits the data Y best in a least-squares sense. P is a
//    row vector of length N+1 containing the polynomial coefficients in
//    descending powers, P(1)*X^N + P(2)*X^(N-1) +...+ P(N)*X + P(N+1).
polynomialfit(num_window, DEGREE, poly_matrix_x, poly_matrix_y, coeff);

for(int i = DEGREE-1; i >= 0 ; i--) {
    //printf("%lf\n", coeff[i]);
  }

//printf("Polynomial Equation for current lane is : %f x^2 %f x %f \n", coeff[2] , coeff[1] , coeff[0]);



}








