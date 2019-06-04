#pragma once

#include <float.h>
#include <limits.h>
typedef double cvec_float;
typedef long long int cvec_int;
#define CVEC_FLOAT_MAX DBL_MAX
#define CVEC_INT_MAX LLONG_MAX


// cvec.c
cvec_float *cvec_linspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_logspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_apply(cvec_float* in, int len, cvec_float (*f)());
cvec_float *cvec_zeros(int len);
cvec_float *cvec_copy(cvec_float *source, int len);
cvec_float *cvec_diff(cvec_float *x, int len);
cvec_float cvec_max(cvec_float *x, int len);
cvec_float cvec_min(cvec_float *x, int len);
cvec_float cvec_average(cvec_float* in, int len);
cvec_float cvec_sum(cvec_float* in, int len);

// cvec_fourier.c
// these funcs assume input is evenly spaced
cvec_float *cvec_fft(cvec_float* in, int in_len);
cvec_float *cvec_freq(int len, cvec_float dt);
// this is for "real" data with uneven input
void cvec_gaussian_fft(cvec_float* in, int in_len, cvec_float *out_y, cvec_float *out_x, int *out_len);

// cvec_sort.c
int cvec_in_order(cvec_float *in, int len);
cvec_float *cvec_insertion_sort(cvec_float *unsorted, int len);
cvec_float *cvec_merge_sort(cvec_float *unsorted, int len);
cvec_float *cvec_quick_sort(cvec_float *unsorted, int len);
cvec_float *cvec_bubble_sort(cvec_float *unsorted, int len);
cvec_float *cvec_sort(cvec_float *unsorted, int len);

// cvec_int.c
cvec_int *cvec_int_linspace(cvec_int from, cvec_int to, int len);
cvec_int *cvec_int_logspace(cvec_int from, cvec_int to, int len);
cvec_int *cvec_int_apply(cvec_int* in, int len, cvec_int (*f)());
cvec_int *cvec_int_zeros(int len);
cvec_int *cvec_int_copy(cvec_int *source, int len);
cvec_int *cvec_int_diff(cvec_int *x, int len);
cvec_int cvec_int_max(cvec_int *x, int len);
cvec_int cvec_int_min(cvec_int *x, int len);
cvec_int cvec_int_average(cvec_int* in, int len);
cvec_int cvec_int_sum(cvec_int* in, int len);

//cvec_stats.c
void cvec_hist(cvec_float *input, int len, cvec_float **output, cvec_float **bins, int *nbins);
cvec_float cvec_std(cvec_float *x, int len);
