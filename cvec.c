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
cvec_apply2(cvec_float *in1, cvec_float *in2, int len, cvec_float (*f)(cvec_float, cvec_float))
{
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = f(in1[i], in2[i]);
  }
  return rv;
}




cvec_float *
cvec_zeros(int len)
{
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (int i = 0; i < len; i++){
    rv[i] = 0.0;
  }
  return rv;
}



cvec_float *
cvec_copy(cvec_float *source, int len)
{
  cvec_float *rv = malloc(sizeof(cvec_float)*len);
#pragma omp parallel for
  for(int i = 0; i < len; i++) {
    rv[i] = source[i];
  }
  return rv;
}



cvec_float *
cvec_cat(cvec_float *source, int len, cvec_float *add, int addlen)
{
  cvec_float *rv = malloc(sizeof(cvec_float)*(len+addlen));

#pragma omp parallel for
  for(int i = 0; i < len; i++) {
    rv[i] = source[i];
  }

#pragma omp parallel for
  for(int i = len; i < (len+addlen); i++) {
    rv[i] = add[i-len];
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




cvec_float *
cvec_add(cvec_float* x, cvec_float *y, int len) {
  cvec_float *rv = malloc(sizeof(cvec_float)*(len-1));
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = x[i] + y[i];
  }
  return rv;
}




cvec_float *
cvec_subtract(cvec_float* x, cvec_float *y, int len) {
  cvec_float *rv = malloc(sizeof(cvec_float)*(len-1));
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = x[i] - y[i];
  }
  return rv;
}




cvec_float *
cvec_multiply(cvec_float* x, cvec_float *y, int len) {
  cvec_float *rv = malloc(sizeof(cvec_float)*(len-1));
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = x[i] * y[i];
  }
  return rv;
}




cvec_float *
cvec_divide(cvec_float* x, cvec_float *y, int len)
{
  cvec_float *rv = malloc(sizeof(cvec_float)*(len-1));
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    rv[i] = x[i] / y[i];
  }
  return rv;
}



cvec_float *
cvec_slice(cvec_float *x, int len, int start, int stop, int skip)
{
  int olen = (stop - start) / skip;
  cvec_float *rv = malloc(sizeof(cvec_float)*olen);
  for (int i = start, j = 0; i < stop; i += skip, j++) {
    rv[j] = x[i];
  }
  return rv;
}




cvec_float
cvec_get_sumse(cvec_float *x, cvec_float *y, int len, cvec_float *coefs, int ncoefs)
{
  cvec_float calc_se(cvec_float o, cvec_float g) { 
    return pow(o - g, 2.0); 
  }
  cvec_float calcfit(cvec_float xi) { 
    cvec_float rv = 0.0; 
    for (int i = 0; i < ncoefs; i++) 
      rv += coefs[i] * pow(xi, (double)i); 
    return rv; 
  }
  cvec_float *t = cvec_apply(x, len, &calcfit);
  cvec_float *se = cvec_apply2(y, t, len, &calc_se);
  cvec_float sumse = cvec_sum(se, len);
  free(t);
  free(se);
  return sumse;
}




cvec_float *
cvec_polyfit(cvec_float* x, cvec_float* y, int len, int degree)
{
  int ncoefs = degree+1;
  cvec_float *coefs = malloc(ncoefs*sizeof(cvec_float));
  for (int i = 0; i < ncoefs; i++) {
    coefs[i] = 1.0;
  }

  // changes to make to coefs
  cvec_float changes[ncoefs];
  for (int i = 0; i < ncoefs; i++) changes[i] = 0.1;

  // for each coeff, make change, analyze if that helped, change change appropriately
  for (int oc = 0; oc < 10000; oc++) {
    for (int i = ncoefs - 1; i >= 0; i--) {
      for (int ic = 0; ic < 10; ic++) {
        if (changes[i] < 0.0001) changes[i] = 0.0001;
        cvec_float err1 = cvec_get_sumse(x, y, len, coefs, ncoefs);
        coefs[i] += changes[i];
        cvec_float err2 = cvec_get_sumse(x, y, len, coefs, ncoefs);
        cvec_float derr = err2 - err1;
        if (derr > 0.0) {
          coefs[i] -= changes[i];
          changes[i] *= -0.99;
          fprintf(stderr, "error getting worse (%f -> %f), reversing (dC = %f)\n", err1, err2, changes[i]);
        }
        else {
          changes[i] *= 1.0;
          fprintf(stderr, "error getting better (%f -> %f), speeding up (dC = %f)\n", err1, err2, changes[i]);
        }
      }
    }
  }

  return coefs;
}




cvec_float *
cvec_linearfit(cvec_float *x, cvec_float *y, int len)
{
  cvec_float midx = cvec_average(x, len);
  cvec_float midy = cvec_average(y, len);

  cvec_float get_int(cvec_float m) { return midy - (m*midx); }

  cvec_float get_sumse(cvec_float *x, cvec_float *y, int len, cvec_float gradient) {
    cvec_float interrupt = get_int(gradient);
    cvec_float applyfit(cvec_float v) { return interrupt+(gradient*v); }
    cvec_float *fity = cvec_apply(x, len, &applyfit);
    cvec_float getse(cvec_float y1, cvec_float y2) {
      return pow(y1 - y2, 2.0);
    }
    cvec_float *se = cvec_apply2(y, fity, len, &getse);
    cvec_float sumse = cvec_sum(se, len);
    free(se);
    free(fity);
    return sumse;
  }

  cvec_float change = 0.1, m = 1.0;
  for (int i = 0; i < len+100; i++) {
    cvec_float err1 = get_sumse(x, y, len, m);
    m += change;
    cvec_float err2 = get_sumse(x, y, len, m);
    if (err2 > err1) {
      m -= change;
      change *= -0.8;
    }
    else {
      change *= 1.2;
    }
  }
  cvec_float *coefs = malloc(sizeof(cvec_float)*2);
  coefs[0] = get_int(m);
  coefs[1] = m;
  return coefs;
}




cvec_float
cvec_max(cvec_float* x, int len)
{
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
