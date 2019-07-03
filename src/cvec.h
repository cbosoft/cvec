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

// flag stuff
#define FLAGSET(A,F) ((A&F)==F)

#define CVEC_PEAKS_POSITIVE 0
#define CVEC_PEAKS_NEGATIVE 1

#define CVEC_LIMITS_MAXIMUM 0
#define CVEC_LIMITS_MINIMUM 1
#define CVEC_LIMITS_THRESHOLD 2

// macros {{{

#define CVEC_PI 3.14159265359

#define FG_BLACK   "\033[30m"
#define FG_RED     "\033[31m"
#define FG_GREEN   "\033[32m"
#define FG_YELLOW  "\033[33m"
#define FG_BLUE    "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN    "\033[36m"
#define FG_WHITE   "\033[37m"

#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

#define RESET "\033[0m"
#define BOLD "\033[1m"

// }}}
// vector.c {{{

// creating new vectors {{{

cvec_float *cvec_linspace(cvec_float from, cvec_float to, cvec_uint len);
cvec_float *cvec_logspace(cvec_float from, cvec_float to, cvec_uint len);
cvec_float *cvec_zeros(cvec_uint len);
cvec_float *cvec_copy(cvec_float *source, cvec_uint len);

// }}}
// simple vector manipulation {{{
cvec_float cvec_add(cvec_float v1, cvec_float v2);
cvec_float cvec_subtract(cvec_float v1, cvec_float v2);
cvec_float cvec_multiply(cvec_float v1, cvec_float v2);
cvec_float cvec_divide(cvec_float v1, cvec_float v2);
cvec_float cvec_exp(cvec_float v1, cvec_float v2);
cvec_float *cvec_apply(cvec_float* in, cvec_uint len, cvec_float (*f)());
cvec_float *cvec_apply2(cvec_float* in1, cvec_float *in2, cvec_uint len, cvec_float (*f)());
cvec_float *cvec_slice(cvec_float *source, cvec_uint len, cvec_uint start, cvec_uint stop, cvec_uint skip);
cvec_float *cvec_cat(cvec_float *source, cvec_uint len, cvec_float *add, cvec_uint addlen);
cvec_float *cvec_diff(cvec_float *x, cvec_uint len);

// }}}
// properties of vector {{{

cvec_float cvec_get_sumse(cvec_float *x, cvec_float *y, cvec_uint len, cvec_float *coefs, cvec_uint ncoefs);
cvec_float *cvec_polyfit(cvec_float *x, cvec_float *y, cvec_uint len, cvec_uint degree);
cvec_float *cvec_linearfit(cvec_float *x, cvec_float *y, cvec_uint len);
cvec_float cvec_interpolate(cvec_float *x, cvec_float *y, cvec_uint len, cvec_float ix);
// }}}

// }}}
// error.c {{{

void cvec_ferr(const char *source, const char* fmt, ...);
void cvec_warn(const char *source, const char* fmt, ...);

// }}}
// limits.c {{{

cvec_float cvec_max(cvec_float *x, cvec_uint len);
cvec_float cvec_min(cvec_float *x, cvec_uint len);
void cvec_get_limit(cvec_float *in, cvec_uint len, cvec_int flags, cvec_float threshold, cvec_float *limit, cvec_uint *index);

// }}}
// fourier.c {{{

// these funcs assume input is evenly spaced
cvec_float *cvec_fft(cvec_float* in, cvec_uint in_len);
cvec_float *cvec_freq(cvec_uint len, cvec_float dt);

// this is for "real" data with uneven input
void cvec_gaussian_fft(cvec_float* in, cvec_uint in_len, cvec_float *out_y, cvec_float *out_x, cvec_uint *out_len);

// }}}
// sort.c {{{

bool cvec_in_order(cvec_float *in, cvec_uint len);
cvec_float *cvec_insertion_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_merge_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_quick_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_bubble_sort(cvec_float *unsorted, cvec_uint len);
cvec_float *cvec_sort(cvec_float *unsorted, cvec_uint len);

// }}}
// int_vector.c {{{

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

// }}}
// stats.c {{{

void cvec_hist(cvec_float *input, cvec_uint len, cvec_float **output, cvec_float **bins, cvec_uint *nbins);
cvec_float cvec_std(cvec_float *x, cvec_uint len);
void cvec_autocorr(cvec_float* x, cvec_float *y, int len, cvec_float **res_x, cvec_float **res_y, int *nbins);
cvec_float cvec_average(cvec_float *in, cvec_uint len);
cvec_float cvec_mean(cvec_float *in, cvec_uint len);
cvec_float cvec_median(cvec_float *in, cvec_uint len);
cvec_float cvec_sum(cvec_float *in, cvec_uint len);
cvec_float cvec_prod(cvec_float *in, cvec_uint len);

// }}}
// matrices.c {{{

cvec_float cvec_matgen_zeros(cvec_uint r, cvec_uint c);
cvec_float cvec_matgen_ones(cvec_uint r, cvec_uint c);
cvec_float cvec_matgen_random(cvec_uint r, cvec_uint c);
cvec_float cvec_matgen_identity(cvec_uint r, cvec_uint c);
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

// }}}
// filter.c {{{

cvec_float *cvec_moving_average(cvec_float *x, cvec_uint len, cvec_uint w, cvec_float (*avfunc)(cvec_float *x, cvec_uint len));

// }}}
// io.c {{{

void cvec_write_csv(cvec_float **data, cvec_uint ncols, cvec_uint *nlines, const char *csv_path);
void cvec_write_csv_xy(cvec_float *x, cvec_float *y, cvec_uint len, const char *csv_path);

// }}}
// signal_proc.c {{{

void cvec_peakdet(cvec_float *signal, cvec_uint len, cvec_float threshhold, cvec_float **peaks, cvec_uint **peak_indices, cvec_uint *npeaks, cvec_int flags);

// }}}
// vim: ft=c foldmethod=marker
