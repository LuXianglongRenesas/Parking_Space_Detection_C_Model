#ifndef _READ_BMP_H_
#define _READ_BMP_H_

//#define image_size 1024

//define size of input bmp image
//*******************************
//INITIALIZATION OF ALL PARAMETERS HERE


#define I_Width 200
#define I_Length 300
#define Radon_rSize 363
#define radon_angle_range 60
#define randon_angle_offset 30
#define min_lane_spacing 10
// #define scalling_factor_width 3/I_Width
// #define scalling_factor_length 6/I_Length
typedef struct{

	int R;
	int G;
	int B;

} image_matrix;

int read_bmp();
int get_size_of_output();

#endif
