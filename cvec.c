#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cvec.h"

double *cvec_linspace(double from, double to, int len) {
  double *rv = malloc(len*sizeof(double));
  double step = (to - from) / ((double)(len - 1));
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((double)i)) + from;
  }
  return rv;
}

double *cvec_logspace(double from, double to, int len) {
  double lfrom = log(from), lto = log(to);
  double *rv = cvec_linspace(lfrom, lto, len);
  for (int i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }
  return rv;
}

double *cvec_apply(double* in, int len, double (*f)(double)) {
  double *rv = malloc(len*sizeof(double));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}

double cvec_average(double* in, int len) {
  double sum = cvec_sum(in, len);
  return sum / ((double)len);
}

double cvec_sum(double* in, int len) {
  double sum = 0.0;
  for (int i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}


int *cvec_int_linspace(int from, int to, int len) {
  int *rv = malloc(len*sizeof(int));
  int step = (to - from) / ((int)(len - 1));
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((int)i)) + from;
  }
  return rv;
}




int *cvec_int_logspace(int from, int to, int len) {
  int lfrom = log(from), lto = log(to);
  int *rv = cvec_int_linspace(lfrom, lto, len);
  for (int i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }
  return rv;
}



int *cvec_int_apply(int* in, int len, int (*f)()) {
  int *rv = malloc(len*sizeof(int));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}


int cvec_int_average(int* in, int len) {
  int sum = cvec_int_sum(in, len);
  return sum / len;
}

int cvec_int_sum(int* in, int len) {
  int sum = 0.0;
  for (int i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}
