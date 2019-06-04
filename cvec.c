#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cvec.h"




cvec_float *
cvec_linspace(cvec_float from, cvec_float to, int len) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));
  cvec_float step = (to - from) / ((cvec_float)(len - 1));

#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((cvec_float)i)) + from;
  }

  return rv;
}




cvec_float *
cvec_logspace(cvec_float from, cvec_float to, int len) {
  cvec_float lfrom = log(from), lto = log(to);
  cvec_float *rv = cvec_linspace(lfrom, lto, len);

#pragma omp parallel for
  for (int i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}




cvec_float *
cvec_apply(cvec_float* in, int len, cvec_float (*f)(cvec_float)) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}




cvec_float *
cvec_zeros(int len) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = 0.0;
  }
  return rv;
}



cvec_float *
cvec_copy(cvec_float * source, int len) {
  cvec_float *rv = malloc(sizeof(cvec_float)*len);
#pragma omp parallel for
  for(int i = 0; i < len; i++) {
    rv[i] = source[i];
  }
  return rv;
}




cvec_float *
cvec_diff(cvec_float* x, int len) {
  cvec_float *rv = malloc(sizeof(cvec_float)*(len-1));
  int i, j;
#pragma omp parallel for
  for (i=0; i < (len-1); i++) {
    j = i+1;
    rv[i] = x[j] - x[i];
  }
  return rv;
}




cvec_float
cvec_max(cvec_float* x, int len) {
  cvec_float rv = -CVEC_FLOAT_MAX;
  for (int i = 0; i < len; i++) {
    if (x[i] > rv) {
      rv = x[i];
    }
  }
  return rv;
}




cvec_float
cvec_min(cvec_float* x, int len) {
  cvec_float rv = CVEC_FLOAT_MAX;
  for (int i = 0; i < len; i++) {
    if (x[i] < rv) {
      rv = x[i];
    }
  }
  return rv;
}




cvec_float
cvec_average(cvec_float* in, int len) {
  cvec_float sum = cvec_sum(in, len);
  return sum / ((cvec_float)len);
}




cvec_float
cvec_sum(cvec_float* in, int len) {
  cvec_float sum = 0.0;
  for (int i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}
