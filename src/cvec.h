#pragma once

#include <float.h>
#include <limits.h>

typedef double cvec_float;
typedef int cvec_int;
typedef unsigned int cvec_uint;
typedef int bool;

typedef cvec_float cvf;
typedef cvec_int cvi;
typedef cvec_uint cvu;

#define CVEC_FLOAT_MAX DBL_MAX
#define CVEC_FLOAT_MIN DBL_MIN
#define CVEC_INT_MAX INT_MAX
#define CVEC_INT_MIN INT_MIN

#define CVEC_CORR_LONG_LEN 100000
#define CVEC_NJOBS 32

// flag stuff
#define FLAGSET(A,F) ((A&F)==F)

#define CVEC_PEAKS_POSITIVE 0
#define CVEC_PEAKS_NEGATIVE 1

#define CVEC_LIMITS_MAXIMUM 0
#define CVEC_LIMITS_MINIMUM 1
#define CVEC_LIMITS_THRESHOLD 2

// macros

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





// vector_template.c

#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N
#include "vector_template.h"
#undef CVECH_TYPE
#undef CVECH_

#define CVECH_TYPE cvec_int
#define CVECH_(N) cvec_int_ ## N
#include "vector_template.h"
#undef CVECH_TYPE
#undef CVECH_

cvec_uint *cvec_uint_linspace(cvec_uint from, cvec_uint to, cvec_uint len);
cvec_uint *cvec_uint_logspace(cvec_uint from, cvec_uint to, cvec_uint len);
cvec_uint *cvec_uint_apply(cvec_uint* in, cvec_uint len, cvec_uint (*f)());
cvec_uint *cvec_uint_zeros(cvec_uint len);
cvec_uint *cvec_uint_copy(cvec_uint *source, cvec_uint len);
cvec_uint *cvec_uint_diff(cvec_uint *x, cvec_uint len);
cvec_uint cvec_uint_max(cvec_uint *x, cvec_uint len);
cvec_uint cvec_uint_min(cvec_uint *x, cvec_uint len);
cvec_uint cvec_uint_average(cvec_uint* in, cvec_uint len);
cvec_uint cvec_uint_sum(cvec_uint* in, cvec_uint len);





// error.c

void cvec_ferr(const char *source, const char* fmt, ...);
void cvec_warn(const char *source, const char* fmt, ...);
void cvec_test_fail(const char *testname, const char* fmt, ...);
void cvec_test_pass(const char *testname, const char* fmt, ...);





// limits_template.c

#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N
#include "limits_template.h"
#undef CVECH_TYPE
#undef CVECH_

#define CVECH_TYPE cvec_int
#define CVECH_(N) cvec_int_ ## N
#include "limits_template.h"
#undef CVECH_TYPE
#undef CVECH_




// fourier.c

// these funcs assume input is evenly spaced
// cvec_float *cvec_fft(cvec_float* in, cvec_uint in_len);
cvec_float *cvec_freq(cvec_uint len, cvec_float dt);

// this is for "real" data with uneven input
void cvec_gaussian_fft(cvec_float* in, cvec_uint in_len, cvec_float **out_y, cvec_float **out_x, cvec_uint *out_len);





// sort_template.c

#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N
#include "sort_template.h"
#undef CVECH_TYPE
#undef CVECH_

#define CVECH_TYPE cvec_int
#define CVECH_(N) cvec_int_ ## N
#include "sort_template.h"
#undef CVECH_TYPE
#undef CVECH_





// stats_template.c

#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N
#include "stats_template.h"
#undef CVECH_TYPE
#undef CVECH_

#define CVECH_TYPE cvec_int
#define CVECH_(N) cvec_int_ ## N
#include "stats_template.h"
#undef CVECH_TYPE
#undef CVECH_





// matrices.c

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






// filter_template.c

#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N
#include "filter_template.h"
#undef CVECH_TYPE
#undef CVECH_

#define CVECH_TYPE cvec_int
#define CVECH_(N) cvec_int_ ## N
#include "filter_template.h"
#undef CVECH_TYPE
#undef CVECH_






// io.c

void cvec_write_csv(cvec_float **data, cvec_uint ncols, cvec_uint *nlines, const char *csv_path);
void cvec_write_csv_xy(cvec_float *x, cvec_float *y, cvec_uint len, const char *csv_path);




// signal_proc.c

void cvec_peakdet(cvec_float *signal, cvec_uint len, cvec_float threshhold, cvec_float **peaks, cvec_uint **peak_indices, cvec_uint *npeaks, cvec_int flags);



// display.c

void progressbar(const char *message, cvec_float progress);
// vim: ft=c
