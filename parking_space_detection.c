//PARKING SPACE DETECTION
#include "parking_space_detection.h"
#include "sobel.h"

#define PI 3.1415926

int img_sobel[I_Width][I_Length];
int image_wt_logic[I_Width][I_Length];
double P_2d[Radon_rSize][radon_angle_range];
int num_marker;
int perp_index;
int parallel_line_index[20];
int perp_line_index[2];
int rSize;
int ct_width = I_Width / 2;
int ct_length = I_Length / 2;
double val = PI/180;
int max_num_parking_space = 10;
int num_perp_line = 0;
int num_parall_line = 0;
int init_perp_index;


extern int bound_array[80]; //max 10 parking space can be detected , limitation here
extern int num_parking_space; //max 10 spaces

//int num_peak_pt = num_marker -1;
//int num_local_max = num_marker;

int abs_peak_array[20][2]; //max length of array

int peak_point_filter()
{
	int cnt = 0;
	//int perp_index;
	int perp_flag = 0;
	printf("number of marker detected is : %d \n",num_marker );
	//printf("abs_peak_array is : %d \n", abs_peak_array[0][0]);
	for (int i = 0; i < num_marker; i++)
	{
		if(abs(abs_peak_array[i][1]) > 180 - randon_angle_offset/2 && abs_peak_array[i][1] < 180 + randon_angle_offset/2)
		{
          perp_index = i;
          printf("perp_index value is : %d \n", perp_index);
          perp_line_index[cnt] = perp_index;
          //we are assuming there;s only one perp lane or none at all
          cnt = cnt + 1;

          if(cnt == 1)
          	init_perp_index = perp_index;

          num_perp_line = num_perp_line + 1;
		}
		printf("init_perp_index value is %d \n", init_perp_index);

	}
		// if(cnt >= 1)
  //         {
  //         	perp_index = 0;
  //         }

		if(cnt == 0)
          {
          	perp_flag = 1; //false flag , goes to case 2!
          	printf("There's no perp line detected!");
          	//perp_index = NULL;
          }

         if(num_perp_line == 1 && abs_peak_array[perp_line_index[0]][0] > rSize/2)
         	{
          	perp_flag = 1; //false flag , goes to case 2!
          	printf("There's incompleted parallel parking spaces detected!\n\n");
          	//perp_index = NULL;
          }

	return perp_flag;
}

void parking_space_detection() //there're two cases
{
	int perp_flag = peak_point_filter();
	int flag = 0;
	int swap;
	int radon_offset = ceil(rSize/2);
	int num_peak_pt = num_marker - num_perp_line;
	num_parall_line = num_marker - num_perp_line;
	int num_peak_pt_no_perp = num_marker;
	int num_local_max = num_marker;
	//num_parking_space = num_peak_pt - 1;

    FILE *f = fopen("crop_image_coordinate_array.txt" , "wb");
    fprintf(f,"%d\n",num_peak_pt - 1);
    
	num_parking_space = num_peak_pt - 1;
		int perp_cnt = 0;
		int para_cnt = 0;
		int flag_1,flag_2;
		// FILE *f = fopen("crop_image_coordinate_array.txt" , "wb");
		// fprintf(f,"%d\n",num_peak_pt_no_perp - 1);
		//printf("complete parking space detected and perp_index is : %d \n", perp_index);
		//compute intersections and store then as matrixs
		// int num_peak_pt = num_marker -1;
		// int num_local_max = num_marker;

		for (int i = 0; i < num_marker; i++)
		{
			flag_1 = 0;
			flag_2 = 0;

			for (int j = 0; j < num_perp_line; j++){

			if(perp_line_index[j] != i){
				flag_1 = 1;
				//parallel_line_index[para_cnt] = i;
				//para_cnt = para_cnt + 1;
				//if(perp_cnt >= 1)
					//parallel_line_index[i] = i;
			}
			if(perp_line_index[j] == i){

				flag_2 = 1;
				//parallel_line_index[i] = i + 1;
				perp_cnt = perp_cnt + 1;
				//break;
			}


			if(flag_1 == 1 && flag_2 == 0 && j == num_perp_line-1)
			{
				//para_cnt = para_cnt + 1;
				parallel_line_index[para_cnt] = i;
				para_cnt = para_cnt + 1;
			}

			if(flag_2 == 1 && j == num_perp_line-1)
			{
				// para_cnt = para_cnt + 1;
				// parallel_line_index[i] = i+1;
			}
	
		    }
		// if(flag)
		// 	continue;
		    printf("para_cnt value is %d \n", para_cnt);
		    if(para_cnt == num_parall_line + 1)
				break;
			//printf("display all parallel lines abs_peak_array parallel_line_index[%d] is %d: \n" ,para_cnt, parallel_line_index[para_cnt]);
	    }
		for (int i = 0; i < num_parall_line; i++)
		{
			printf("display all parallel lines abs_peak_array parallel_line_index[%d] is %d: \n" ,i, parallel_line_index[i]);
			//printf("display all parallel lines abs_peak_array is %d \n" , parallel_line_index[i]);
			//sort this array 
		}

	switch(perp_flag)
	{
		case 0 :
		num_parking_space = num_peak_pt - 1;
		int perp_cnt = 0;
		int para_cnt = 0;
		int flag_1,flag_2;
		// FILE *f = fopen("crop_image_coordinate_array.txt" , "wb");
		// fprintf(f,"%d\n",num_peak_pt_no_perp - 1);
		//printf("complete parking space detected and perp_index is : %d \n", perp_index);
		//compute intersections and store then as matrixs
		// int num_peak_pt = num_marker -1;
		// int num_local_max = num_marker;

		for (int i = 0; i < num_marker; i++)
		{
			flag_1 = 0;
			flag_2 = 0;

			for (int j = 0; j < num_perp_line; j++){

			if(perp_line_index[j] != i){
				flag_1 = 1;
				//parallel_line_index[para_cnt] = i;
				//para_cnt = para_cnt + 1;
				//if(perp_cnt >= 1)
					//parallel_line_index[i] = i;
			}
			if(perp_line_index[j] == i){

				flag_2 = 1;
				//parallel_line_index[i] = i + 1;
				perp_cnt = perp_cnt + 1;
				//break;
			}


			if(flag_1 == 1 && flag_2 == 0 && j == num_perp_line-1)
			{
				//para_cnt = para_cnt + 1;
				parallel_line_index[para_cnt] = i;
				para_cnt = para_cnt + 1;
			}

			if(flag_2 == 1 && j == num_perp_line-1)
			{
				// para_cnt = para_cnt + 1;
				// parallel_line_index[i] = i+1;
			}
	
		    }
		// if(flag)
		// 	continue;
		    printf("para_cnt value is %d \n", para_cnt);
		    if(para_cnt == num_parall_line + 1)
				break;
			//printf("display all parallel lines abs_peak_array parallel_line_index[%d] is %d: \n" ,para_cnt, parallel_line_index[para_cnt]);
	    }
		for (int i = 0; i < num_parall_line; i++)
		{
			printf("display all parallel lines abs_peak_array parallel_line_index[%d] is %d: \n" ,i, parallel_line_index[i]);
			//printf("display all parallel lines abs_peak_array is %d \n" , parallel_line_index[i]);
			//sort this array 
		}
		// 	for (int j = i + 1; j < num_parall_line; j++)

		// 	if(parallel_line_index[i] > parallel_line_index[j])
		// 	{
		// 		swap = parallel_line_index[i];
		// 		parallel_line_index[i] = parallel_line_index[j];
		// 		parallel_line_index[j] = swap;
		// 	}

			//printf("display all parallel lines abs_peak_array is %d \n" , parallel_line_index[i]);
		//}
		
		//polar - cartisian coordinate conversion 
		//intersect points calculation

		//calculate perp line equation first
		double x_2,y_2,offset_perp;
		double slope_a_perp,b_perp;
		int bound_insec_x, bound_insec_y;
		int temp_x,temp_y;
		

		abs_peak_array[init_perp_index][0] = abs_peak_array[init_perp_index][0] - radon_offset;
		offset_perp = abs_peak_array[init_perp_index][0];
		x_2 = ct_length + offset_perp*sin((abs_peak_array[init_perp_index][1])*val);
		y_2 = -(ct_width + offset_perp*sin((abs_peak_array[init_perp_index][1] - 90)*val));
		slope_a_perp = tan((abs_peak_array[init_perp_index][1])*val);


		b_perp = y_2 - slope_a_perp*x_2;
		printf("slope of perp line is: %f \n", slope_a_perp);
		//printf("b_perp value is : %f \n", b_perp);
		printf("y_2 value is : %f \n", y_2);
		//printf("value check (abs_peak_array[parallel_line_index[perp_index]][1]) = %d \n" , (abs_peak_array[parallel_line_index[perp_index]][1]));

		for (int i = 0; i < num_parall_line; i++)
		{	
			double offset = 0;
			double x_1 = 0;
			double y_1 = 0;
			double slope_a_paral = 0;
			double b_paral = 0;
			double intersect_x = 0;
			double intersect_y = 0;
			double intersect_2_x;
			double intersect_2_y;
			int flag_temp_value_check = 0;

			if(abs_peak_array[parallel_line_index[i]][1] > 90){
				abs_peak_array[parallel_line_index[i]][1] = abs(abs_peak_array[parallel_line_index[i]][1] - 180);
			}

			abs_peak_array[parallel_line_index[i]][0] = abs_peak_array[parallel_line_index[i]][0] - radon_offset;
			offset = abs_peak_array[parallel_line_index[i]][0];
			x_1 = ct_length + offset*sin((abs_peak_array[parallel_line_index[i]][1])*val);
			y_1 = -(ct_width - offset*sin((abs_peak_array[parallel_line_index[i]][1])*val));
			//printf("display all parallel lines abs_peak_array is %d \n" , abs_peak_array[parallel_line_index[i]][0]);  checked 
			printf("x_1, y_1 value check point : x_1 = %f and y_1 = %f \n", x_1,y_1);//checked pass

			//we assume each line is in format of y = ax + b with slope =  a 
	
			slope_a_paral = tan((abs_peak_array[parallel_line_index[i]][1])*val);
			b_paral = y_1 - slope_a_paral * x_1;

			//calculate each intersect point and store in array
			if((slope_a_paral - slope_a_perp) != 0){
			intersect_x = (b_perp - b_paral)/(slope_a_paral - slope_a_perp);
			intersect_y = -(slope_a_paral*intersect_x + b_paral);

			if(intersect_x == 0)
				printf("error , no intersect point detected ! \n");

			//printf("b_paral value is : %f \n", b_paral);
			//printf("x_1 value is : %f \n", x_1);
			//printf("y_1 value is : %f \n", y_1);
			printf("slope of parallel line is: %f \n", slope_a_paral);
			//printf("intersect x value is: %f \n", intersect_x);
			//printf("intersect y value is: %f \n", intersect_y);
			printf("intersect point coordinate is [%f][%f]\n", intersect_x,intersect_y);
			//printf("value check (abs_peak_array[parallel_line_index[i]][1])*val = %f \n" , (abs_peak_array[parallel_line_index[i]][1]*val));
			//printf("b value check : b = %f \n", b_paral);
			bound_array[i*4 + 2] = intersect_x;
			bound_array[i*4 + 3] = intersect_y;

			//detect intersect points with image boundaries
			//left bound

			//b_paral = -b_paral;
			//printf("b value check : b = %f \n", b_paral);
			//printf("value check : %f \n", (-I_Width - b_paral)/slope_a_paral);

			//lower bound
			if((-I_Width - b_paral)/slope_a_paral > 0 && (-I_Width - b_paral)/slope_a_paral <  I_Length)
			{
				bound_insec_x = (-I_Width - b_paral)/slope_a_paral;
				bound_insec_y = I_Width;
				printf("case checked 1\n");
			}
			//right bound
			else if(b_paral + slope_a_paral*I_Length > -I_Width && b_paral + slope_a_paral*I_Length < 0)
			{
				bound_insec_x = I_Length;
				bound_insec_y = -(b_paral + slope_a_paral*I_Length);
				printf("case checked 2 \n");
			}
			//left bound
			else if(b_paral > -I_Width && b_paral < 0)
			{
				bound_insec_x = 0;
				bound_insec_y = -b_paral;
				printf("case checked 3 \n");
			}
			printf("boundary intersect point coordinate are [%d][%d]\n", bound_insec_x,bound_insec_y);

			//we have bound_insec_x and bound_insec_y, then we can check for the other two end points 
			for(int i = bound_insec_y; i > intersect_y ; i--)
			{
				if(flag_temp_value_check == 1)
					break;

				for(int j = -sweep_sensitivity; j <= sweep_sensitivity ; j++)
				{
					temp_y = i;
					temp_x = (- temp_y - b_paral)/slope_a_paral + j;
					//printf("temp value check [temp_x][temp_y]: [%d][%d] \n", temp_x,temp_y);
					if(temp_x < 0)
						temp_x = 0;
					else if(temp_x > I_Length)
						temp_x = I_Length;

					if(image_wt_logic[I_Width - temp_y][temp_x] == 255)
					{   //printf("value check [temp_y][temp_x] are : [%d][%d] \n", temp_y,temp_x);
						//printf("temp value check [temp_x][temp_y]: [%d][%d] \n", temp_x,temp_y);
						intersect_2_x = (- temp_y - b_paral)/slope_a_paral;
						intersect_2_y = temp_y;
						flag_temp_value_check = 1;
					}

					if(flag_temp_value_check == 1)
					break;
				}

				// if(flag_temp_value_check == 1)
				// 	break;
			}
			//printf("second intersect point coordinate is [%f][%f]\n", intersect_2_x,intersect_2_y);
			bound_array[i*4 ] = intersect_2_x;
			bound_array[i*4 + 1] = intersect_2_y;

			}

		}


		// for(int i = 0; i < num_peak_pt - 1; i++) 
		// {
		// 	for(int j = i+1; j < num_peak_pt - 1; j++){
		// 	//sort this array , in order left to right
		// 		//printf("loop check \n");
		// 		for (int k = 0; k < 4; k++){
		// 	if(bound_array[j*4] < bound_array[i*4])
		// 		{
		// 		swap = bound_array[i*4+k];
		// 		bound_array[i*4+k] = bound_array[j*4+k];
		// 		bound_array[j*4+k] = swap;
		// 		//printf("swap function call check \n");
		// 		}
		// 		}
		// 	}
		// }

		for(int i = 0; i < num_peak_pt - 1; i++) 
		{
		printf("Parking space detection is performed \n");

		printf("pixel boundary of parking sace number %d is : [%d][%d];[%d][%d];[%d][%d];[%d][%d] \n"
			, i+1,bound_array[i*4+2],bound_array[i*4+3],bound_array[(i+1)*4+2],bound_array[(i+1)*4+3],bound_array[(i+1)*4],
			bound_array[(i+1)*4 + 1],bound_array[i*4],bound_array[i*4+1]);

		//writing to file
		fprintf(f,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",bound_array[i*4+2],bound_array[i*4+3],bound_array[(i+1)*4+2],bound_array[(i+1)*4+3],bound_array[(i+1)*4],
			bound_array[(i+1)*4 + 1],bound_array[i*4],bound_array[i*4+1]);
		}
		fclose(f);
		break;





		case 1 :
		num_parking_space = num_parall_line - 1;
		//ANOTHER CASE STARTS HERE!

		printf("non-complete parking space detected, further detection needed \n\n");
		// FILE *f = fopen("crop_image_coordinate_array.txt" , "wb");
		// fprintf(f,"%d\n",num_peak_pt_no_perp - 1);
		// for (int i = 0; i < num_peak_pt_no_perp; i++)
		// {
		// 	if(perp_index != i){
		// 		parallel_line_index[i] = i;
		// 		if(flag == 1)
		// 			parallel_line_index[i] = i + 1;
		// 	}
		// 	if(perp_index == i){
		// 		flag = 1;
		// 		parallel_line_index[i] = i + 1;
		// 	}
		// 	//printf("display all parallel lines abs_peak_array is %d: \n" , parallel_line_index[i]);
		// }
		// for (int i = 0; i < num_peak_pt_no_perp; i++)
		// {
		// 	//printf("value perp_index is %d \n", perp_index);
		// 	//printf("display all parallel lines abs_peak_array is %d \n" , parallel_line_index[i]);
		// 	//sort this array 

		// 	for (int j = i + 1; j < num_peak_pt_no_perp; j++)

		// 	if(parallel_line_index[i] > parallel_line_index[j])
		// 	{
		// 		swap = parallel_line_index[i];
		// 		parallel_line_index[i] = parallel_line_index[j];
		// 		parallel_line_index[j] = swap;
		// 	}

		// 	//printf("display all parallel lines abs_peak_array is %d \n" , parallel_line_index[i]);
		// }
		
		//polar - cartisian coordinate conversion 
		//intersect points calculation

		//calculate perp line equation first
		// double x_2,y_2,offset_perp;
		// double slope_a_perp,b_perp;
		// int bound_insec_x, bound_insec_y;
		// int temp_x,temp_y;
		

		// abs_peak_array[perp_index][0] = abs_peak_array[perp_index][0] - radon_offset;
		// offset_perp = abs_peak_array[perp_index][0];
		// x_2 = ct_length + offset_perp*sin((90 - abs_peak_array[perp_index][1])*val);
		// y_2 = -(ct_width - offset_perp*sin((abs_peak_array[perp_index][1])*val));
		// slope_a_perp = tan((abs_peak_array[perp_index][1] - 90)*val);


		// b_perp = y_2 - slope_a_perp*x_2;
		//printf("slope of perp line is: %f \n", slope_a_perp);
		//printf("b_perp value is : %f \n", b_perp);
		//printf("x_2 value is : %f \n", x_2);
		//printf("value check (abs_peak_array[parallel_line_index[perp_index]][1]) = %d \n" , (abs_peak_array[parallel_line_index[perp_index]][1]));

		for (int i = 0; i < num_parall_line ; i++)
		{	
			double offset = 0;
			double x_1 = 0;
			double y_1 = 0;
			double slope_a_paral = 0;
			double b_paral = 0;
			double intersect_x = 0;
			double intersect_y = 0;
			double intersect_2_x;
			double intersect_2_y;
			int flag_temp_value_check = 0;

		 //    if(abs_peak_array[parallel_line_index[i]][1] > 90){
			// 	abs_peak_array[parallel_line_index[i]][1] = abs_peak_array[parallel_line_index[i]][1] - 180;
			// }

			abs_peak_array[parallel_line_index[i]][0] = abs_peak_array[parallel_line_index[i]][0] - radon_offset;
			offset = abs_peak_array[parallel_line_index[i]][0];
			x_1 = ct_length + offset*sin((abs_peak_array[parallel_line_index[i]][1])*val);
			y_1 = -(ct_width - offset*cos((abs_peak_array[parallel_line_index[i]][1])*val));
			//printf("display all parallel lines abs_peak_array is %d \n" , abs_peak_array[parallel_line_index[i]][0]);  checked 
			printf("x_1, y_1 value check point : x_1 = %f and y_1 = %f \n", x_1,y_1);//checked pass

			//we assume each line is in format of y = ax + b with slope =  a 
			slope_a_paral = tan((abs_peak_array[parallel_line_index[i]][1])*val);
			b_paral = y_1 - slope_a_paral * x_1;

			//calculate each intersect point and store in array
			if((slope_a_paral - slope_a_perp) != 0){
			intersect_x = (b_perp - b_paral)/(slope_a_paral - slope_a_perp);
			intersect_y = -(slope_a_paral*intersect_x + b_paral);

			if(intersect_x == 0)
				printf("error , no intersect point detected ! \n");

			//printf("b_paral value is : %f \n", b_paral);
			//printf("x_1 value is : %f \n", x_1);
			//printf("y_1 value is : %f \n", y_1);
			printf("slope of parallel line is: %f \n", slope_a_paral);
			//printf("intersect x value is: %f \n", intersect_x);
			//printf("intersect y value is: %f \n", intersect_y);
			//printf("intersect point coordinate is [%f][%f]\n", intersect_x,intersect_y);
			//printf("value check (abs_peak_array[parallel_line_index[i]][1])*val = %f \n" , (abs_peak_array[parallel_line_index[i]][1]*val));
			//printf("b value check : b = %f \n", b_paral);
			bound_array[i*4 + 2] = intersect_x;
			bound_array[i*4 + 3] = intersect_y;

			//detect intersect points with image boundaries
			//left bound

			//b_paral = -b_paral;
			//printf("b value check : b = %f \n", b_paral);
			//printf("value check : %f \n", (-I_Width - b_paral)/slope_a_paral);

			//lower bound
			if((-I_Width - b_paral)/slope_a_paral > 0 && (-I_Width - b_paral)/slope_a_paral <  I_Length)
			{
				bound_insec_x = (-I_Width - b_paral)/slope_a_paral;
				bound_insec_y = I_Width;
				printf("case checked 1\n");
			}
			//right bound
			else if(b_paral + slope_a_paral*I_Length > -I_Width && b_paral + slope_a_paral*I_Length < 0)
			{
				bound_insec_x = I_Length;
				bound_insec_y = -(b_paral + slope_a_paral*I_Length);
				printf("case checked 2 \n");
			}
			//left bound
			else if(b_paral > -I_Width && b_paral < 0)
			{
				bound_insec_x = 0;
				bound_insec_y = -b_paral;
				printf("case checked 3 \n");
			}
			printf("boundary intersect point coordinate are [%d][%d]\n", bound_insec_x,bound_insec_y);

			//we have bound_insec_x and bound_insec_y, then we can check for the other two end points 
			for(int i = bound_insec_y; i > intersect_y ; i--)
			{
				if(flag_temp_value_check == 1)
					break;

				for(int j = -sweep_sensitivity; j <= sweep_sensitivity ; j++)
				{
					temp_y = i;
					temp_x = (- temp_y - b_paral)/slope_a_paral + j;
					//printf("temp value check [temp_x][temp_y]: [%d][%d] \n", temp_x,temp_y);
					if(temp_x < 0)
						temp_x = 0;
					else if(temp_x > I_Length)
						temp_x = I_Length;

					if(image_wt_logic[I_Width - temp_y][temp_x] == 255)
					{   //printf("value check [temp_y][temp_x] are : [%d][%d] \n", temp_y,temp_x);
						//printf("temp value check [temp_x][temp_y]: [%d][%d] \n", temp_x,temp_y);
						intersect_2_x = (- temp_y - b_paral)/slope_a_paral;
						intersect_2_y = temp_y;
						flag_temp_value_check = 1;
					}

					if(flag_temp_value_check == 1)
					break;
				}

				// if(flag_temp_value_check == 1)
				// 	break;
			}
			//printf("second intersect point coordinate is [%f][%f]\n", intersect_2_x,intersect_2_y);
			bound_array[i*4 ] = intersect_2_x;
			bound_array[i*4 + 1] = intersect_2_y;

			}

		}

		for(int i = 0; i < num_parking_space; i++) 
		{
		printf("Parking space detection is performed \n");

		// printf("pixel boundary of parking sace number %d is : [%d][%d];[%d][%d];[%d][%d];[%d][%d] \n"
		// 	, i+1,bound_array[i*4],bound_array[i*4+1],bound_array[i*4+2],bound_array[i*4+3],bound_array[(i+1)*4],
		// 	bound_array[(i+1)*4+1],bound_array[(i+1)*4+2],bound_array[(i+1)*4+3]);
		// }

//corrcted order for python code

		printf("pixel boundary of parking space number %d is : [%d][%d];[%d][%d];[%d][%d];[%d][%d] \n"
			, i+1,bound_array[i*4+2],bound_array[i*4+3],bound_array[(i+1)*4+2],bound_array[(i+1)*4+3],bound_array[(i+1)*4],
			bound_array[(i+1)*4 + 1],bound_array[i*4],bound_array[i*4+1]);
		

//writing to file
		fprintf(f,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",bound_array[i*4+2],bound_array[i*4+3],bound_array[(i+1)*4+2],bound_array[(i+1)*4+3],bound_array[(i+1)*4],
			bound_array[(i+1)*4 + 1],bound_array[i*4],bound_array[i*4+1]);

		}
    //writing array to txt file as input of python code
	
	 //    fopen("crop_image_coordinate_array.txt", "rb");

	 //    fread(bound_array, sizeof(char),80,f);

	 //    for(int i=0;i<80;i++)
	 //    {
	 //        (fgets (bound_array[i], 1, f));
	 //    }
	 //    fclose(f);
		break;
	fclose(f);
	}

}