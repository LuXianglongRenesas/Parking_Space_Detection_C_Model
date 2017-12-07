#ifndef HIST_EQ_H
#define HIST_EQ_H

#include "configuration.h"

void histogram_equalization(char RGB_Channel);

void histogram_equalization_sliding_window();

int apply_histogram_equalization(int *data, int *data_out , unsigned int size);

#endif