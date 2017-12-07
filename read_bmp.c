#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*void read_bmp() //old way to read bmp, not used now, now read info from header
{
 extern int image[image_size][3]; // first number here is 1024 pixels in my image, 3 is for RGB values
 FILE *streamIn;
 streamIn = fopen("./white_lane_marker_undisortion.bmp", "r");
 
 static int i;
 static int byte;
 static int count = 0;

 for(i=0;i<54;i++) byte = getc(streamIn);  // strip out BMP header

 for(i=0;i<image_size;i++){    // foreach pixel
    image[i][2] = getc(streamIn);  // use BMP 24bit with no alpha channel
    image[i][1] = getc(streamIn);  // BMP uses BGR but we want RGB, grab byte-by-byte
    image[i][0] = getc(streamIn);  // reverse-order array indexing fixes RGB issue...
    printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
 }

 fclose(streamIn);

 //return image;
}*/

// Set to 1 to use BMP format, 0 to use RAW format
#define BMP_FORMAT_USED 1

int rSize;

//initiate img matrix for yuv,rgb,hsv color space 
extern image_matrix image_matrix_bmp[I_Width][I_Length];
extern image_matrix_rgb matrix_rgb[I_Width][I_Length];
extern image_matrix_hsv matrix_hsv[I_Width][I_Length];
extern int max_R_value;
extern int avg_Y;


int read_bmp()
{
    int i;
#if BMP_FORMAT_USED
#define BMP_HEADER_SIZE 54
    //FILE* f = fopen("./Pics/white_lane_marker_undisortion.bmp", "rb");
    FILE* f = fopen("./Pics/topView_input_resized_dark.bmp", "rb");
    //FILE* f = fopen("./Pics/topView_input_resized.bmp", "rb");
    //FILE* f = fopen("./Pics/fake_parking_spaces/ideal_parallel.bmp", "rb");

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
#else
#define BMP_HEADER_SIZE 0
    FILE* f = fopen("./Pics/topView_input_resized_dark.yuv", "rb");
    int width = I_Length;
    int height = I_Width;
#endif

    int size = 3 * I_Width * I_Length + BMP_HEADER_SIZE;
    unsigned char* image;
    image = malloc(size); // allocate 3 bytes per pixel
    fread(image, sizeof(unsigned char), size, f); // read the rest of the pixel data at once
    fclose(f);

    int count = 1;
    for(int i = BMP_HEADER_SIZE; i < size; i += 3)
    {
            unsigned char tmp = image[i];
            image[i] = image[i + 2];
            image[i+2] = tmp;
            count = count + 1;
           // printf("RGB value of pixel %d is [%d %d %d] \n", count - 1 , image[i] , image[i+1], image[i+2]);
    }
    //int image_array[width][height];

    //image_matrix image_matrix_bmp[height][width];
    //image_matrix *image_matrix_bmp = malloc(I_Width*I_Length);
    //image_matrix *image_matrix_bmp = malloc(I_Width*I_Length*sizeof(image_matrix));
    //

    int sum_Y = 0;
    avg_Y = 0;

    //printf("image_matrix [I_Width][I_Length] is [%d, %d,%d]" , image_matrix_bmp[I_Width][I_Length].R, image_matrix_bmp[I_Width][I_Length].G, image_matrix_bmp[I_Width][I_Length].B);
    
    //bmp version
    #if BMP_FORMAT_USED
    for(int j = 0; j < I_Width; j++)
    {
    	for(int i = 0 ; i < I_Length ; i++)
    	{                                        //wired offset here, 84,85,86 for 900x600 bmp
    		image_matrix_bmp[j][i].Y = image[(j)*I_Length*3 + (i)*3 + 0]; 
    		image_matrix_bmp[j][i].U = image[(j)*I_Length*3 + (i)*3 + 1]; 
    		image_matrix_bmp[j][i].V = image[(j)*I_Length*3 + (i)*3 + 2]; 

            sum_Y = sum_Y + image[(j)*I_Length*3 + (i)*3 + 0];
    	//printf("YUV value of pixel [%d,%d] is [%d %d %d] \n", j , i , image_matrix_bmp[j][i].Y, image_matrix_bmp[j][i].U, image_matrix_bmp[j][i].V);
    	}
    }
    //yuv version 
    #else
    for(int j = 0; j < I_Width; j++)
    {
        for(int i = 0 ; i < I_Length ; i++)
        {                                        //wired offset here, 84,85,86 for 900x600 bmp
            image_matrix_bmp[I_Width-j][i].V = image[(j)*I_Length*3 + (i)*3 + 0]; 
            image_matrix_bmp[I_Width-j][i].U = image[(j)*I_Length*3 + (i)*3 + 1]; 
            image_matrix_bmp[I_Width-j][i].Y = image[(j)*I_Length*3 + (i)*3 + 2]; 

            sum_Y = sum_Y + image[(j)*I_Length*3 + (i)*3 + 2];
        //printf("YUV value of pixel [%d,%d] is [%d %d %d] \n", j , i , image_matrix_bmp[j][i].Y, image_matrix_bmp[j][i].U, image_matrix_bmp[j][i].V);
        }
    }
    #endif

    // for(int j = 0; j < I_Width; j++)
    // {
    //     for(int i = 0 ; i < I_Length ; i++)
    //     {  
    //         matrix_rgb[I_Width-j][i].R = (image_matrix_bmp[j][i].Y + 1.13983*(image_matrix_bmp[j][i].V-128))/2;
    //         matrix_rgb[I_Width-j][i].G = (image_matrix_bmp[j][i].Y - 0.39465*(image_matrix_bmp[j][i].U-128) - 0.58060*(image_matrix_bmp[j][i].V-128))/2;
    //         matrix_rgb[I_Width-j][i].B = (image_matrix_bmp[j][i].Y + 2.03211*(image_matrix_bmp[j][i].U-128))/2;
    //         //printf("RGB value of pixel [%d,%d] is [%d %d %d] \n", j , i , matrix_rgb[j][i].R , matrix_rgb[j][i].G , matrix_rgb[j][i].B);
    //     }
    // }
    // printf("RGB value of pixel [%d,%d] is [%d %d %d] \n", 37 , 261 , matrix_rgb[37][261].R , matrix_rgb[37][261].G , matrix_rgb[37][261].B);

    //Another rgb to yuv approach, checked 
     for(int j = 0; j < I_Width; j++)
    {
        for(int i = 0 ; i < I_Length ; i++)
        {  
            matrix_rgb[j][i].R = 1.164*(image_matrix_bmp[j][i].Y - 16) + 1.596*(image_matrix_bmp[j][i].V-128);
            matrix_rgb[j][i].G = 1.164*(image_matrix_bmp[j][i].Y - 16) - 0.391*(image_matrix_bmp[j][i].U-128) - 0.813*(image_matrix_bmp[j][i].V-128);
            matrix_rgb[j][i].B = 1.164*(image_matrix_bmp[j][i].Y - 16) + 2.018*(image_matrix_bmp[j][i].U-128);
            //printf("RGB value of pixel [%d,%d] is [%d %d %d] \n", j , i , matrix_rgb[j][i].R , matrix_rgb[j][i].G , matrix_rgb[j][i].B);
        }
    }
    //printf("RGB value of pixel [%d,%d] is [%d %d %d] \n", 175 , 175 , matrix_rgb[175][175].R , matrix_rgb[175][175].G , matrix_rgb[175][175].B);

    //To find max RGB values for hist_eq algo
    max_R_value = 0;

    for(int j = 0; j < I_Width; j++)
    {
        for(int i = 0 ; i < I_Length ; i++)
        {  
            if(matrix_rgb[j][i].R > max_R_value)
                max_R_value = matrix_rgb[j][i].R;
        }
    }

    avg_Y = sum_Y/(I_Width*I_Length);
    printf("average Y value is %d \n\n", avg_Y); //tried to handle lighting issue


    rSize = get_size_of_output();
    return rSize;
}


    //calculate rSize in read_bmp.c
    int get_size_of_output()
{
    //int rSize;
    int Width = I_Width;
    int Length = I_Length;
    int M, N;               /* input image size*/ 
    int xOrigin, yOrigin;   /* center of image*/ 
    int temp1, temp2;       /* used in output size computation*/      
    int rFirst, rLast;      /* r-values for first and last row of output*/
     //size of input image
    M = Width;
    N = Length;
    //origin of image
    xOrigin = N/2;
    yOrigin = M/2;

    //size of output
    temp1 = M - 1 - yOrigin;     
    temp2 = N - 1 - xOrigin;     
    rLast = (int) ceil(sqrt((double) (temp1*temp1+temp2*temp2))) + 1;     
    rFirst = -rLast;     
    rSize = rLast - rFirst + 3;   

    return rSize;
} 



    	

