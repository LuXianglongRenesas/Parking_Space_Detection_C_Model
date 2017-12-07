#include "configuration.h"
#include "bmp_crop.h"
#include <stdio.h>

int bound_array[80];
int num_parking_space;

int cropped_bound_array[80];
/* Calculates coefficients of perspective transformation
 * which maps (xi,yi) to (ui,vi), (i=1,2,3,4):
 *
 *      c00*xi + c01*yi + c02
 * ui = ---------------------
 *      c20*xi + c21*yi + c22
 *
 *      c10*xi + c11*yi + c12
 * vi = ---------------------
 *      c20*xi + c21*yi + c22
 *
 * Coefficients are calculated by solving linear system:
 * / x0 y0  1  0  0  0 -x0*u0 -y0*u0 \ /c00\ /u0\
 * | x1 y1  1  0  0  0 -x1*u1 -y1*u1 | |c01| |u1|
 * | x2 y2  1  0  0  0 -x2*u2 -y2*u2 | |c02| |u2|
 * | x3 y3  1  0  0  0 -x3*u3 -y3*u3 |.|c10|=|u3|,
 * |  0  0  0 x0 y0  1 -x0*v0 -y0*v0 | |c11| |v0|
 * |  0  0  0 x1 y1  1 -x1*v1 -y1*v1 | |c12| |v1|
 * |  0  0  0 x2 y2  1 -x2*v2 -y2*v2 | |c20| |v2|
 * \  0  0  0 x3 y3  1 -x3*v3 -y3*v3 / \c21/ \v3/
 *
 * where:
 *   cij - matrix coefficients, c22 = 1  
     TBD in 2018 , convert warpPerspective to C */ 

void bmp_crop(){

//initialize real time coordinate variables 
	int tl_x,tl_y,tr_x,tr_y,lr_x,lr_y,ll_x,ll_y,left_x,right_x,top_y,bottom_y;

	for(int i = 0; i < num_parking_space; i++)
	{
		//calculate all 4 corner coordinates 
		tl_x = bound_array[i*4+2];
		tl_y = bound_array[i*4+3];
		tr_x = bound_array[(i+1)*4+2];
		tr_y = bound_array[(i+1)*4+3];
		lr_x = bound_array[(i+1)*4];
		lr_y = bound_array[(i+1)*4+1];
		ll_x = bound_array[i*4];
		ll_y = bound_array[i*4+1];

		//take the minimum rectangle
		if(tl_x <= ll_x)
		left_x = ll_x;
	    else left_x = tl_x;

	    if(tr_x <= lr_x)
		right_x = tr_x;
	    else right_x = lr_x;

	    if(tl_y >= tr_y)
		top_y = tl_y;
	    else top_y = tr_y;

	    if(ll_y >= lr_y)
		bottom_y = lr_y;
	    else bottom_y = ll_y;

        cropped_bound_array[i*4+2] = left_x;
        cropped_bound_array[i*4+3] = top_y;
        cropped_bound_array[(i+1)*4+2] = right_x;
        cropped_bound_array[(i+1)*4+3] = top_y;
        cropped_bound_array[(i+1)*4] = right_x;
        cropped_bound_array[(i+1)*4+1] = bottom_y;
        cropped_bound_array[i*4] = left_x;
        cropped_bound_array[i*4+1] = bottom_y;

        double delta_length = ((tr_x-tl_x)+(lr_x-ll_x))/2  - (right_x - left_x);
        double delta_length_err = delta_length/((tr_x-tl_x)+(lr_x-ll_x))/2;
        double delta_width = ((lr_y-tr_y)+(ll_y-tl_y))/2  - (bottom_y - top_y);
        double delta_width_err = delta_width/((lr_y-tr_y)+(ll_y-tl_y))/2;

        double cropped_img_confidency = (1-delta_length_err)*(1-delta_width_err);


//evaluation 
    printf("\n");
	printf("cropped pixel boundary of parking space number %d is : [%d][%d];[%d][%d];[%d][%d];[%d][%d] \n"
			, i+1,cropped_bound_array[i*4+2],cropped_bound_array[i*4+3],cropped_bound_array[(i+1)*4+2],cropped_bound_array[(i+1)*4+3],cropped_bound_array[(i+1)*4],
			cropped_bound_array[(i+1)*4 + 1],cropped_bound_array[i*4],cropped_bound_array[i*4+1]);
	printf("cropped imgage confidency of parking space no. %d is %f %% \n", i+1,cropped_img_confidency*100);

}
}