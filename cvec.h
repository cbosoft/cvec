#pragma once

#include <float.h>
#include <limits.h>
typedef double cvec_float;
typedef long long int cvec_int;
#define CVEC_FLOAT_MAX DBL_MAX
#define CVEC_INT_MAX LLONG_MAX

#define CVEC_PI 3.14159265359
// cvec.c
cvec_float *cvec_linspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_logspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_apply(cvec_float* in, int len, cvec_float (*f)());
cvec_float *cvec_zeros(int len);
cvec_float *cvec_copy(cvec_float *source, int len);
cvec_float *cvec_slice(cvec_float *source, int len, int start, int stop, int skip);
cvec_float *cvec_cat(cvec_float *source, int len, cvec_float *add, int addlen);
cvec_float *cvec_diff(cvec_float *x, int len);
cvec_float *cvec_add(cvec_float *x, cvec_float *y, int len);
cvec_float *cvec_subtract(cvec_float *x, cvec_float *y, int len);
cvec_float *cvec_multiply(cvec_float *x, cvec_float *y, int len);
cvec_float *cvec_divide(cvec_float *x, cvec_float *y, int len);
cvec_float cvec_get_sumse(cvec_float *x, cvec_float *y, int len, cvec_float *coefs, int ncoefs);
cvec_float *cvec_polyfit(cvec_float *x, cvec_float *y, int len, int degree);
cvec_float *cvec_linearfit(cvec_float *x, cvec_float *y, int len);
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

// cvec_stats.c
void cvec_hist(cvec_float *input, int len, cvec_float **output, cvec_float **bins, int *nbins);
cvec_float cvec_std(cvec_float *x, int len);

// cvec_matrix.c
cvec_float **cvec_matrix_new(int rows, int cols, cvec_float (*f)(int, int));
cvec_float **cvec_matrix_cross(cvec_float **A, int rA, int cA, cvec_float **B, int rB, int cB);
cvec_float **cvec_matrix_copy(cvec_float **A, int R, int C);
cvec_float **cvec_matrix_apply(cvec_float **A, int R, int C, cvec_float (*f)(cvec_float));
cvec_float **cvec_matrix_invert(cvec_float **A, int R, int C);
cvec_float **cvec_matrix_minor(cvec_float **A, int R, int C, int notR, int notC);
cvec_float **cvec_matrix_of_minors(cvec_float **A, int R, int C);
cvec_float **cvec_matrix_of_cofactors(cvec_float **A, int R, int C);
cvec_float **cvec_matrix_transpose(cvec_float **A, int R, int C);
cvec_float cvec_matrix_determinant(cvec_float **A, int R, int C);
int cvec_matrix_is_invertible(cvec_float **A, int R, int C);
void cvec_print_matrix(cvec_float **A, int R, int C);
void cvec_matrix_free(cvec_float **A, int R, int C);

// cvec_filter.c
cvec_float *cvec_moving_average(cvec_float *x, int len, int w);

// vim: ft=c
