#include <stdlib.h>
#include <math.h>

#include "cvec.h"

cvec_int *
cvec_int_linspace(cvec_int from, cvec_int to, int len) {
  cvec_int *rv = malloc(len*sizeof(cvec_int));
  cvec_int step = (to - from) / ((cvec_int)(len - 1));

#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((cvec_int)i)) + from;
  }

  return rv;
}




cvec_int *
cvec_int_logspace(cvec_int from, cvec_int to, int len) {
  cvec_int lfrom = log(from), lto = log(to);
  cvec_int *rv = cvec_int_linspace(lfrom, lto, len);

#pragma omp parallel for
  for (int i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}



cvec_int *
cvec_int_apply(cvec_int* in, int len, cvec_int (*f)(cvec_int)) {
  cvec_int *rv = malloc(len*sizeof(cvec_int));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}




cvec_int *
cvec_int_zeros(int len) {
  cvec_int *rv = malloc(len*sizeof(cvec_int));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = 0;
  }
  return rv;
}




cvec_int *
cvec_int_copy(cvec_int * source, int len) {
  cvec_int *rv = malloc(sizeof(cvec_int)*len);
#pragma omp parallel for
  for(int i = 0; i < len; i++) {
    rv[i] = source[i];
  }
  return rv;
}




cvec_int *
cvec_int_diff(cvec_int* x, int len) {
  cvec_int *rv = malloc(sizeof(cvec_int)*(len-1));
  int i, j;
#pragma omp parallel for
  for (i=0; i < (len-1); i++) {
    j = i+1;
    rv[i] = x[j] - x[i];
  }
  return rv;
}




cvec_int
cvec_int_max(cvec_int* x, int len) {
  cvec_int rv = -CVEC_INT_MAX;
  for (int i = 0; i < len; i++) {
    if (x[i] > rv) {
      rv = x[i];
    }
  }
  return rv;
}




cvec_int
cvec_int_min(cvec_int* x, int len) {
  cvec_int rv = CVEC_INT_MAX;
  for (int i = 0; i < len; i++) {
    if (x[i] < rv) {
      rv = x[i];
    }
  }
  return rv;
}




cvec_int 
cvec_int_average(cvec_int* in, int len) {
  cvec_int sum = cvec_int_sum(in, len);
  return sum / len;
}




cvec_int
cvec_int_sum(cvec_int* in, int len) {
  cvec_int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}
