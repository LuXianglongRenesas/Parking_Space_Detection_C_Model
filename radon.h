#ifndef _RADON_H_
#define _RADON_H_

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "configuration.h"

//initialization of functions
//int get_size_of_output();

void radon_transform(int I[I_Width][I_Length], double* THETA);

void incrementRadon(double *pr, double pixel, double r);

static void radon(double *pPtr, double *iPtr, double *thetaPtr, int M, int N,      
      int xOrigin, int yOrigin, int numAngles, int rFirst, int rSize);

static int P_local_peak(double P_2d[Radon_rSize][radon_angle_range], int window_size, int angle_window_size, int local_max_num, int threshold);

#endif
