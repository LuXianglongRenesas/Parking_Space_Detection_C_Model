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
#define min_lane_spacing 35
#define sweep_sensitivity 30
#define lighting_scale_factor 2
#define hist_eq_window_length 20
#define hist_eq_window_width 10
#define hist_eq_threshold 220
#define num_window_hori 15
#define num_window_verti 20
#define color_filter_thres 240


// #define scalling_factor_width 3/I_Width
// #define scalling_factor_length 6/I_Length
typedef struct{

	int Y;
	int U;
	int V;

} image_matrix;

typedef struct{

	int R;
	int G;
	int B;

} image_matrix_rgb;

typedef struct{

	int H;
	int S;
	int V;

} image_matrix_hsv;

int read_bmp();
int get_size_of_output();

#endif
