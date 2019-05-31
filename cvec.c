#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cvec.h"

cvec_float *cvec_linspace(cvec_float from, cvec_float to, int len) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));
  cvec_float step = (to - from) / ((cvec_float)(len - 1));

#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((cvec_float)i)) + from;
  }

  return rv;
}

cvec_float *cvec_logspace(cvec_float from, cvec_float to, int len) {
  cvec_float lfrom = log(from), lto = log(to);
  cvec_float *rv = cvec_linspace(lfrom, lto, len);

#pragma omp parallel for
  for (int i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}

cvec_float *cvec_apply(cvec_float* in, int len, cvec_float (*f)(cvec_float)) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}

cvec_float cvec_average(cvec_float* in, int len) {
  cvec_float sum = cvec_sum(in, len);
  return sum / ((cvec_float)len);
}

cvec_float cvec_sum(cvec_float* in, int len) {
  cvec_float sum = 0.0;
  for (int i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}


cvec_int *cvec_int_linspace(cvec_int from, cvec_int to, int len) {
  cvec_int *rv = malloc(len*sizeof(cvec_int));
  cvec_int step = (to - from) / ((cvec_int)(len - 1));

#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((cvec_int)i)) + from;
  }

  return rv;
}




cvec_int *cvec_int_logspace(cvec_int from, cvec_int to, int len) {
  cvec_int lfrom = log(from), lto = log(to);
  cvec_int *rv = cvec_int_linspace(lfrom, lto, len);

#pragma omp parallel for
  for (int i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}



cvec_int *cvec_int_apply(cvec_int* in, int len, cvec_int (*f)(cvec_int)) {
  cvec_int *rv = malloc(len*sizeof(cvec_int));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}


cvec_int cvec_int_average(cvec_int* in, int len) {
  cvec_int sum = cvec_int_sum(in, len);
  return sum / len;
}

cvec_int cvec_int_sum(cvec_int* in, int len) {
  cvec_int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}
