/*
 *  Name: Asher Christian
 *  UID: 006-150-286
 */

#include <stdlib.h>
#include <omp.h>

#include "utils.h"
#include "parallel.h"

/*
 *  PHASE 1: compute the mean pixel value
 *  This code is buggy! Find the bug and speed it up.
 */
void mean_pixel_parallel(const uint8_t img[][NUM_CHANNELS], int num_rows, int num_cols, double mean[NUM_CHANNELS])
{
    int row, col, ch;
    long count = num_rows*num_cols;
    double m0 = 0, m1 = 0, m2 = 0;

	#pragma omp parallel for collapse(2) reduction(+:m0, m1, m2)
	for (row = 0; row < num_rows; row++)
	{
	    for (col = 0; col < num_cols; col++)
	    {
		m0 += img[row * num_cols + col][0];
		m1 += img[row * num_cols + col][1];
		m2 += img[row * num_cols + col][2];
	    }
	}
    mean[0] = m0/count;
    mean[1] = m1/count;
    mean[2] = m2/count;
}

/*
 *  PHASE 2: convert image to grayscale and record the max grayscale value along with the number of times it appears
 *  This code is NOT buggy, just sequential. Speed it up.
 */
void grayscale_parallel(const uint8_t img[][NUM_CHANNELS], int num_rows, int num_cols, uint32_t grayscale_img[][NUM_CHANNELS], uint8_t *max_gray, uint32_t *max_count)
{
    int mg = 0;
    int mc = 0;
#pragma omp parallel
{
    int val;
    int row, col, ch, gray_ch;
    int local_c = 0;
    int local_g = 0;
    int id = omp_get_thread_num();
    int nthrds = omp_get_num_threads();
    for (row = id; row < num_rows; row+=nthrds)
    {
        for (col = 0; col < num_cols; col++)
        {
		val = 0;
                for (ch = 0; ch < NUM_CHANNELS; ch++)
                {
                   val += img[row * num_cols + col][ch];
                }
		val /= NUM_CHANNELS;
		for (gray_ch = 0; gray_ch < NUM_CHANNELS; gray_ch++)
		{
			grayscale_img[row * num_cols + col][gray_ch] = val;
		}
                if (val == local_g)
                {
			local_c += NUM_CHANNELS;
                }
                else if (val > local_g)
                {
                    local_g = val;
                    local_c = NUM_CHANNELS;
                }
        }
    }
    #pragma omp critical
    {
	if(local_g > mg){
		mg = local_g;
		mc = local_c;
	}
	else if(local_g == mg){
		mc += local_c;
	}
    }
}
    *max_count = mc;
    *max_gray = mg;
}

/*
 *  PHASE 3: perform convolution on image
 *  This code is NOT buggy, just sequential. Speed it up.
 */
void convolution_parallel(const uint8_t padded_img[][NUM_CHANNELS], int num_rows, int num_cols, const uint32_t kernel[], int kernel_size, uint32_t convolved_img[][NUM_CHANNELS])
{
    int kernel_norm, i;
    int conv_rows, conv_cols;

    // compute kernel normalization factor
    kernel_norm = 0;
    for (i = 0; i < kernel_size * kernel_size; i++)
    {
        kernel_norm += kernel[i];
    }

    // compute dimensions of convolved image
    conv_rows = num_rows - kernel_size + 1;
    conv_cols = num_cols - kernel_size + 1;

    int row, col;
#pragma omp parallel for collapse(2)
	for (row = 0; row < conv_rows; row++)
	{
		for(col = 0; col < conv_cols; col++)
		{
                        int loc = row * conv_cols + col;
			int kernel_row, kernel_col;
			convolved_img[loc][0] = 0;
			convolved_img[loc][1] = 0;
			convolved_img[loc][2] = 0;
			for (kernel_row = 0; kernel_row < kernel_size; kernel_row++)
			{
				int secloc = (row + kernel_row) * num_cols + col;
				for (kernel_col = 0; kernel_col < kernel_size; kernel_col++)
				{
					int kval = kernel[kernel_row * kernel_size + kernel_col];	
					convolved_img[loc][0] += padded_img[secloc + kernel_col][0] * kval;
					convolved_img[loc][1] += padded_img[secloc + kernel_col][1] * kval;
					convolved_img[loc][2] += padded_img[secloc + kernel_col][2] * kval;
				}
			}
			convolved_img[loc][0] /= kernel_norm;
	                convolved_img[loc][1] /= kernel_norm;
	                convolved_img[loc][2] /= kernel_norm;

		}
	}
}
