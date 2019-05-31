#pragma once

typedef double cvec_float;
typedef long long int cvec_int;



cvec_float *cvec_linspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_logspace(cvec_float from, cvec_float to, int len);
cvec_float *cvec_apply(cvec_float* in, int len, cvec_float (*f)());
cvec_float cvec_average(cvec_float* in, int len);
cvec_float cvec_sum(cvec_float* in, int len);


cvec_int *cvec_int_linspace(cvec_int from, cvec_int to, int len);
cvec_int *cvec_int_logspace(cvec_int from, cvec_int to, int len);
cvec_int *cvec_int_apply(cvec_int* in, int len, cvec_int (*f)());
cvec_int cvec_int_average(cvec_int* in, int len);
cvec_int cvec_int_sum(cvec_int* in, int len);
