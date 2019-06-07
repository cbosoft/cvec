#pragma once

#include <float.h>
#include <limits.h>
typedef double cvec_float;
typedef int cvec_int;
typedef unsigned int cvec_uint;
typedef int bool;
#define CVEC_FLOAT_MAX DBL_MAX
#define CVEC_FLOAT_MIN DBL_MIN
#define CVEC_INT_MAX INT_MAX
#define CVEC_INT_MIN INT_MIN

#define CVEC_PI 3.14159265359

// cvec.c
// creating new vectors
cvec_float *cvec_linspace(cvec_float from, cvec_float to, cvec_uint len);
cvec_float *cvec_logspace(cvec_float from, cvec_float to, cvec_uint len);
cvec_float *cvec_zeros(cvec_uint len);
cvec_float *cvec_copy(cvec_float *source, cvec_uint len);

// simple vector manipulation
cvec_float cvec_add(cvec_float v1, cvec_float v2);
cvec_float cvec_subtract(cvec_float v1, cvec_float v2);
cvec_float cvec_multiply(cvec_float v1, cvec_float v2);
cvec_float cvec_divide(cvec_float v1, cvec_float v2);
cvec_float cvec_exp(cvec_float v1, cvec_float v2);
cvec_float *cvec_apply(cvec_float* in, cvec_uint len, cvec_float (*f)());
cvec_float *cvec_slice(cvec_float *source, cvec_uint len, cvec_uint start, cvec_uint stop, cvec_uint skip);
cvec_float *cvec_cat(cvec_float *source, cvec_uint len, cvec_float *add, cvec_uint addlen);
cvec_float *cvec_diff(cvec_float *x, cvec_uint len);

cvec_float cvec_get_sumse(cvec_float *x, cvec_float *y, cvec_uint len, cvec_float *coefs, cvec_uint ncoefs);
cvec_float *cvec_polyfit(cvec_float *x, cvec_float *y, cvec_uint len, cvec_uint degree);
cvec_float *cvec_linearfit(cvec_float *x, cvec_float *y, cvec_uint len);
cvec_float cvec_max(cvec_float *x, cvec_uint len);
cvec_float cvec_min(cvec_float *x, cvec_uint len);
cvec_float cvec_average(cvec_float* in, cvec_uint len);
cvec_float cvec_sum(cvec_float* in, cvec_uint len);

// cvec_fourier.c
// these funcs assume input is evenly spaced
cvec_float *cvec_fft(cvec_float* in, cvec_uint in_len);
cvec_float *cvec_freq(cvec_uint len, cvec_float dt);
// this is for "real" data with uneven input
void cvec_gaussian_fft(cvec_float* in, cvec_uint in_len, cvec_float *out_y, cvec_float *out_x, cvec_uint *out_len);

// cvec_sort.c
bool cvec_in_order(cvec_float *in, cvec_uint len);
cvec_float *cvec_insertion_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_merge_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_quick_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_bubble_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_sort(cvec_float *unsorted, cvec_uint len);

// cvec_int.c
cvec_int *cvec_int_linspace(cvec_int from, cvec_int to, cvec_uint len);
cvec_int *cvec_int_logspace(cvec_int from, cvec_int to, cvec_uint len);
cvec_int *cvec_int_apply(cvec_int* in, cvec_uint len, cvec_int (*f)());
cvec_int *cvec_int_zeros(cvec_uint len);
cvec_int *cvec_int_copy(cvec_int *source, cvec_uint len);
cvec_int *cvec_int_diff(cvec_int *x, cvec_uint len);
cvec_int cvec_int_max(cvec_int *x, cvec_uint len);
cvec_int cvec_int_min(cvec_int *x, cvec_uint len);
cvec_int cvec_int_average(cvec_int* in, cvec_uint len);
cvec_int cvec_int_sum(cvec_int* in, cvec_uint len);

// cvec_stats.c
void cvec_hist(cvec_float *input, cvec_uint len, cvec_float **output, cvec_float **bins, cvec_uint *nbins);
cvec_float cvec_std(cvec_float *x, cvec_uint len);

// cvec_matrix.c
cvec_float **cvec_matrix_new(cvec_uint rows, cvec_uint cols, cvec_float (*f)(cvec_uint, cvec_uint));
cvec_float **cvec_matrix_cross(cvec_float **A, cvec_uint rA, cvec_uint cA, cvec_float **B, cvec_uint rB, cvec_uint cB);
cvec_float **cvec_matrix_copy(cvec_float **A, cvec_uint R, cvec_uint C);
cvec_float **cvec_matrix_apply(cvec_float **A, cvec_uint R, cvec_uint C, cvec_float (*f)(cvec_float));
cvec_float **cvec_matrix_invert(cvec_float **A, cvec_uint R, cvec_uint C);
cvec_float **cvec_matrix_minor(cvec_float **A, cvec_uint R, cvec_uint C, cvec_uint notR, cvec_uint notC);
cvec_float **cvec_matrix_of_minors(cvec_float **A, cvec_uint R, cvec_uint C);
cvec_float **cvec_matrix_of_cofactors(cvec_float **A, cvec_uint R, cvec_uint C);
cvec_float **cvec_matrix_transpose(cvec_float **A, cvec_uint R, cvec_uint C);
cvec_float cvec_matrix_determinant(cvec_float **A, cvec_uint R, cvec_uint C);
bool cvec_matrix_is_invertible(cvec_float **A, cvec_uint R, cvec_uint C);
void cvec_print_matrix(cvec_float **A, cvec_uint R, cvec_uint C);
void cvec_matrix_free(cvec_float **A, cvec_uint R, cvec_uint C);

// cvec_filter.c
cvec_float *cvec_moving_average(cvec_float *x, cvec_uint len, cvec_uint w);

// vim: ft=c
