/* Radon transform Implementation for lane detection 

Xianglong Lu
Renesas Electronics America 

*syntax [P,r] = radon(I, theta)
*theta range is [0:179]
*we use superposition of the radon transforms of each image pixel

*/
#include "radon.h"
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//extern int rSize;                      //some problems here , how to set rSize value outside of function 
int rSize;
//#define size 1000
//int Size = rSize;
double I_1d[I_Width*I_Length];
double R[Radon_rSize]; 
double P[Radon_rSize*radon_angle_range];
extern double P_2d[Radon_rSize][radon_angle_range];
extern int abs_peak_array[20][2]; //max detect 20 lanes so far , upper bound
extern int num_marker;
//int num_marker = 0;
//int **peak_array = malloc(sizeof(*peak_array) * 10);
//double* pr[rSize*180];

void radon_transform(int I[I_Width][I_Length], double* THETA)
{
    //FILE *fp;
    //fp = fopen("Radon_Output.txt", "w");// "w" means that we are going to write on this file
    int numAngles;          /* number of theta values */     
    double *thetaPtr;       /* pointer to theta values in radians */     
    double *pr1, *pr2;      /* double pointers used in loop */     
    double deg2rad;         /* conversion factor */     
    double temp;            /* temporary theta-value holder */     
    double P_max = 0;
    int k;                  /* loop counter */     
    int M, N;               /* input image size */     
    int xOrigin, yOrigin;   /* center of image */     
    int temp1, temp2;       /* used in output size computation */     
    int rFirst, rLast;      /* r-values for first and last row of output */     
    //int rSize;              /* number of rows in output */  
    int location,P_max_row,P_max_col;
    //double R[rSize]; 
    //double P[rSize*numAngles];

	/* Get THETA values */   
	deg2rad = 3.1415 / 180.0;  
	numAngles = radon_angle_range;
	thetaPtr = (double *) calloc(numAngles, sizeof(double));
	pr1 = THETA;         //first address of THETA array
	pr2 = thetaPtr;       //first address of new transformed data array

	for (k = 0; k < numAngles; k++)     
    {
      *(pr2++) = *(pr1++) * deg2rad;
     // printf("*(pr2) value is :%f  \n", *(pr2));
    }

    //size of input image
  	M = I_Width;
  	N = I_Length;
  	//origin of image
	xOrigin = N/2;
	yOrigin = M/2;

	//size of output
	temp1 = M - 1 - yOrigin;     
    temp2 = N - 1 - xOrigin;     
    rLast = (int) ceil(sqrt((double) (temp1*temp1+temp2*temp2))) + 1;     
    rFirst = -rLast;     
    rSize = rLast - rFirst + 3;   

    //printf("rSize value is : %d , \n" , rSize);

    //calculate output R

    pr1 = &R[0];

    for (k = rFirst; k <= rLast; k++)     
            *(pr1++) = (double) k;   

// convert from 2d array to 1d array

    for(int j = 0; j < I_Length; j++)
    {
    	for(int i = 0 ; i < I_Width ; i++)
    	{
    	  I_1d[i + I_Width*j] = I[I_Width - i][j];
          ////printf("I matrix value check : I[%d][%d] = %d \n", i,j,I[I_Width - i][j]);
    	}
    }  

    //printf("Now start performing RADON transform..\n");

    radon(&P[0], &I_1d[0], thetaPtr, M, N, xOrigin, yOrigin,      
              numAngles, rFirst, rSize);

    //printf("*************DEBUG**************\n\n");
    
    for (int k = 0; k<rSize*radon_angle_range; k++)
    { if (P[k] > P_max)
        {
            P_max = P[k];
            location = k+1;
        }

    }
    P_max_row = location%rSize;
    P_max_col = location/rSize;

    //convert P to 2D matrix
    for(int j = 0; j < radon_angle_range; j++)
    {
        for(int i = 0 ; i < rSize ; i++)
       {
           P_2d[i][j] = P[i + rSize*j];
           //f//printf(fp,"I matrix value check : I[%d][%d] = %d \n", i,j,I[i][j]);
       }
   }

    // visualizations 
    num_marker = P_local_peak(P_2d, 30, 30, 10, 30);

    //for (int k = 0; k<rSize*180; k++) //printf(" %d th P value is %f: \n" , k, P[k]);
    //printf("max value in P[%d][%d] array is %f \n location is %d \n", P_max_row, P_max_col, P_max, location);
    ////printf("P_2d matrix value check : P_2d[586][39] = %f \n" , P_2d[586][39]);
    ////printf(" window_max[41][4] value is: %f \n", window_max[41][4]);
    //writeBMP(P_2d, rSize, 180); 
} 


void incrementRadon(double *pr, double pixel, double r)     //increase accuracy
{     
    int r1;     
    double delta; 
	r1 = (int) r;   
    delta = r - r1;     
    pr[r1] += pixel * (1.0 - delta); 
    pr[r1+1] += pixel * delta;
}  

//Main radon transform function

static void radon(double *pPtr, double *iPtr, double *thetaPtr, int M, int N,      
      int xOrigin, int yOrigin, int numAngles, int rFirst, int rSize)     
{     
    int k, m, n;              /* loop counters */     
    double angle;             /* radian angle value */     
    double cosine, sine;      /* cosine and sine of current angle */     
    double *pr;               /* points inside output array */     
    double *pixelPtr;         /* points inside input array */     
    double pixel;             /* current pixel value */     
    double *ySinTable, *xCosTable;     
    /* tables for x*cos(angle) and y*sin(angle) */     
    double x,y;     
    double r, delta;     
    int r1;  

    /* Allocate space for the lookup tables */ 
    xCosTable = (double *) calloc(2*N, sizeof(double));    
    ySinTable = (double *) calloc(2*M, sizeof(double));  

    for (k = 0; k < numAngles; k++)   
    {     
        angle = thetaPtr[k];     
        pr = pPtr + k*rSize;  /* pointer to the top of the output column */ 
        //pr = pPtr;    
        cosine = cos(angle);      
        sine = sin(angle);        
     
        /* Radon impulse response locus:  R = X*cos(angle) + Y*sin(angle) */     
        /* Fill the X*cos table and the Y*sin table. */
	for (n = 0; n < N; n++)     
        {     
            x = n - xOrigin;     
            xCosTable[2*n]   = (x - 0.25)*cosine;   
            xCosTable[2*n+1] = (x + 0.25)*cosine;     
        }     
        for (m = 0; m < M; m++)     
        {     
            y = yOrigin - m;     
            ySinTable[2*m] = (y - 0.25)*sine;   
            ySinTable[2*m+1] = (y + 0.25)*sine;     
        }     
     
        pixelPtr = iPtr;     

        for (n = 0; n < N; n++)     
        {     
            for (m = 0; m < M; m++)   //go through every pixel in image I 
            {     
                pixel = *pixelPtr++;     
                if (pixel != 0.0)     
                {     
                    pixel *= 0.25;     
                      
                    //each pixel can be devide into 4 near pixels , improve accuracy

                    r = xCosTable[2*n] + ySinTable[2*m] - rFirst;     
                    incrementRadon(pr, pixel, r);     
     
                    r = xCosTable[2*n+1] + ySinTable[2*m] - rFirst;     
                    incrementRadon(pr, pixel, r);     
     
                    r = xCosTable[2*n] + ySinTable[2*m+1] - rFirst;     
                    incrementRadon(pr, pixel, r);     
     
                    r = xCosTable[2*n+1] + ySinTable[2*m+1] - rFirst;     
                    incrementRadon(pr, pixel, r);     
                }     
            }     
        }     
    }     
                     
    free((void *) xCosTable);   //free memory 
    free((void *) ySinTable);     
}

//to find local maximum using sliding window and threshold
// tips, window size can not be larger than either rSize or numAngles

static int P_local_peak(double P_2d[Radon_rSize][radon_angle_range], int window_size, int angle_window_size , int local_max_num, int threshold)
{
    int k,m,n,a,b,c,j,i,o,p;        //loop variables
    int num_marker = 0;
    int num_box;                //number of normal size sliding windows
    double local_window[window_size * angle_window_size];
    double local_max = 0.0;
    //double window_max[b][a];
    int **peak_array = malloc(sizeof(*peak_array) * local_max_num);
    //printf("**********DEBUG MALLOC********* \n\n");
    //int **abs_peak_array = malloc(sizeof(*abs_peak_array) * local_max_num);
    //peak_array = (int[local_max_num][2]){};
    //peak_array = (int *) malloc(local_max_num*2);
    //int peak_array[local_max_num][2];
    double paral_peak_value = 0.0;
    double perp_peak_value = 0.0;

    a = radon_angle_range / angle_window_size;
    b = Radon_rSize / window_size;
    double window_max[b][a];

    num_box = a*b;              // number of normal windows
                                //initialization of 2d array window_max
    //printf("number of box is : %d \n",num_box);
    //double** window_max = 0;
    //for (i = 0; i < b; i++)
    //{ window_max[i] = new double[a];
    //    for (j = 0; j < a; j++)
    //    { 
    for (i = 0; i < b; i++)
    {
        //window_max[b] = (double *)malloc(a);
        local_max = 0.0;

        for (j = 0; j < a/2; j++)
        {

        local_max = 0.0;

            for (n = 0; n < window_size * angle_window_size ; n++) 
               {
                local_window[n] = P_2d[i*window_size + n/angle_window_size][j*window_size + n%angle_window_size];

                //pick local max in each sliding window
                if (local_window[n] > local_max)
                {
                    local_max = local_window[n];
                }

                //printf("local maximum of sliding window [%d][%d] is %f \n" , i,j,local_max);

                }

        window_max[i][j] = local_max;

        ////printf("local maximum of sliding window [%d][%d] is %f \n" , i,j,window_max[i][j]);
        //get max value 
    
        if (window_max[i][j] > paral_peak_value)
        {
            paral_peak_value = window_max[i][j]; //peak value of all windows
        }
        //printf("window_max[%d][%d]is:%f and peak_value is :%f \n",i,j,window_max[i][j], peak_value);
        }

for (j = a/2; j < a; j++)
        {

        local_max = 0.0;

            for (n = 0; n < window_size * angle_window_size ; n++) 
               {
                local_window[n] = P_2d[i*window_size + n/angle_window_size][j*window_size + n%angle_window_size];

                //pick local max in each sliding window
                if (local_window[n] > local_max)
                {
                    local_max = local_window[n];
                }
                //printf("local maximum of sliding window [%d][%d] is %f \n" , i,j,local_max);
                }

        window_max[i][j] = local_max;

        ////printf("local maximum of sliding window [%d][%d] is %f \n" , i,j,window_max[i][j]);

        //get max value 
        
        if (window_max[i][j] > perp_peak_value)
        {
            perp_peak_value = window_max[i][j]; //peak value of all windows
        }
        //printf("window_max[%d][%d]is:%f and peak_value is :%f \n",i,j,window_max[i][j], peak_value);
        }
    } 
    //from this function you got a 2D matrix window_max;

//printf("\n\n\n\n\n\n\n\n\n DEBUG");

for (i = 0; i < b; i++)
    {
        for (j = 0; j < a/2; j++)
        {
	       printf("window_max[%d][%d]is:%f and peak_value is :%f \n",i,j,window_max[i][j], paral_peak_value);
	    }

        for (j = a/2; j < a; j++)
        {
           printf("window_max[%d][%d]is:%f and peak_value is :%f \n",i,j,window_max[i][j], perp_peak_value);
        }

     }
//printf("\n\n\n\n\n\n\n\n\n DEBUG");
int pre_rsize_perp = 0;
int pre_rsize_paral = 0;


for (i = 0; i < b; i++)
    {
        int flag_2 = 0;
        int flag_3 = 0;
        //int pre_rsize = 0;

        for (j = 0; j < a/2; j++) //multiple parallel lanes
        {
            //printf("window_max[%d][%d]is:%f and peak_value is :%f \n",i,j,window_max[i][j], peak_value);
            ////printf("**********ENTER LOOP DEBUG************\n\n");
	       //printf("window_max[i][j]is:%f and peak_value is :%f \n",window_max[i][j], peak_value);
            if(window_max[i][j] >= paral_peak_value/2)  
            {
               printf("******ENTER IF DEBUG******* \n" );
            // if(window_max[i][j] == peak_value)  
            // {
                peak_array[num_marker] = malloc(sizeof(**peak_array));
                //abs_peak_array[num_marker] = malloc(sizeof(**abs_peak_array));

                peak_array[num_marker][0] = i;
                peak_array[num_marker][1] = j;
                //num_marker = num_marker + 1;
                //printf("num_marker value is : %d \n" , num_marker);
                //printf("peak value locates in window [%d][%d] is %f \n", \
                peak_array[num_marker][0], peak_array[num_marker][1],window_max[peak_array[num_marker][0]][peak_array[num_marker][1]]);

                for (m = i*window_size; m < (i+1)*window_size ; m++)
                {
                    int flag = 0;
                        for (n = j*angle_window_size  ; n < (j+1)*angle_window_size ; n++)
                        {
                             //printf("P_2d[%d][%d] value is %f \n", m,n,P_2d[m][n]);
                            if(P_2d[m][n] == window_max[peak_array[num_marker][0]][peak_array[num_marker][1]])
                            {
                                //printf("P_2d[%d][%d] value is %f \n", m,n,P_2d[m][n]);

                                abs_peak_array[num_marker][0] = m;
                                if(abs(m - pre_rsize_perp) < min_lane_spacing){
                                    flag_3 = 1;
                                break;}
                                pre_rsize_perp = m;

                                abs_peak_array[num_marker][1] = n + (90 - randon_angle_offset/2);
                                printf("absolute peak value locates in window [%d][%d] is %f \n", \
                                abs_peak_array[num_marker][0], abs_peak_array[num_marker][1] , P_2d[m][n]);
                                ////printf("detected \n");
                                flag = 1;
                            }
                            //break;
                            if(flag)
                                break;
                        }
                }
                if(flag_3)
                    break;
                num_marker = num_marker + 1;

                flag_2 = 1;
            }
            if(flag_2)
                    break;
            // break;
            // }
            //if(window_max[i][j] < peak_value)
        }

//pre_rsize = 0;

        for (j = a/2; j < a; j++)        //multi perp lanes 
        {
            flag_2 = 0;
            flag_3 = 0;
            //printf("window_max[%d][%d]is:%f and peak_value is :%f \n",i,j,window_max[i][j], peak_value);
            ////printf("**********ENTER LOOP DEBUG************\n\n");
           //printf("window_max[i][j]is:%f and peak_value is :%f \n",window_max[i][j], peak_value);
            if(window_max[i][j] >= perp_peak_value/4)  
            {
               printf("******ENTER IF DEBUG******* \n" );
            // if(window_max[i][j] == peak_value)  
            // {
                peak_array[num_marker] = malloc(sizeof(**peak_array));
                //abs_peak_array[num_marker] = malloc(sizeof(**abs_peak_array));

                peak_array[num_marker][0] = i;
                peak_array[num_marker][1] = j;
                //num_marker = num_marker + 1;
                //printf("num_marker value is : %d \n" , num_marker);
                //printf("peak value locates in window [%d][%d] is %f \n", \
                peak_array[num_marker][0], peak_array[num_marker][1],window_max[peak_array[num_marker][0]][peak_array[num_marker][1]]);

                for (m = i*window_size; m < (i+1)*window_size ; m++)
                {
                    int flag = 0;
                        for (n = j*angle_window_size  ; n < (j+1)*angle_window_size ; n++)
                        {
                             //printf("P_2d[%d][%d] value is %f \n", m,n,P_2d[m][n]);
                            if(P_2d[m][n] == window_max[peak_array[num_marker][0]][peak_array[num_marker][1]])
                            {
                                //printf("P_2d[%d][%d] value is %f \n", m,n,P_2d[m][n]);

                                abs_peak_array[num_marker][0] = m;

                                if(abs(m-pre_rsize_paral) < min_lane_spacing){
                                    flag_3 = 1;
                                break;}

                                pre_rsize_paral = m;

                                abs_peak_array[num_marker][1] = n + (90 + radon_angle_range - randon_angle_offset/2);
                                printf("absolute peak value locates in window [%d][%d] is %f \n", \
                                abs_peak_array[num_marker][0], abs_peak_array[num_marker][1] , P_2d[m][n]);
                                ////printf("detected \n");
                                flag = 1;
                            }
                            //break;
                            if(flag)
                                break;

                        }
                }
                if(flag_3)
                    break;
                num_marker = num_marker + 1;

                flag_2 = 1;
            }
            if(flag_2)
                    break;

        }




    }
    //printf("peak value locates in window [%d][%d] is %f \n", peak_array[1][0], peak_array[1][1],window_max[peak_array[0][0]][peak_array[0][1]]);
    return num_marker;

}
















