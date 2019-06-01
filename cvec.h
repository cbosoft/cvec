#pragma once

typedef double cvec_float;
typedef long long int cvec_int;


// cvec.c
cvec_float *cvec_linspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_logspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_apply(cvec_float* in, int len, cvec_float (*f)());
cvec_float *cvec_zeros(int len);
cvec_float *cvec_copy(cvec_float *source, int len);
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

// cvec_int.c
cvec_int *cvec_int_linspace(cvec_int from, cvec_int to, int len);
cvec_int *cvec_int_logspace(cvec_int from, cvec_int to, int len);
cvec_int *cvec_int_apply(cvec_int* in, int len, cvec_int (*f)());
cvec_int *cvec_int_zeros(int len);
cvec_int cvec_int_average(cvec_int* in, int len);
cvec_int cvec_int_sum(cvec_int* in, int len);
