#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cvec.h"




cvec_float * cvec_linspace(cvec_float from, cvec_float to, cvec_uint len)
{
  cvec_float *rv = malloc(len*sizeof(cvec_float));
  cvec_float step = (to - from) / ((cvec_float)(len - 1));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++) {
    rv[i] = (step*((cvec_float)i)) + from;
  }

  return rv;
}




cvec_float * cvec_logspace(cvec_float from, cvec_float to, cvec_uint len)
{
  cvec_float lfrom = log(from), lto = log(to);
  cvec_float *rv = cvec_linspace(lfrom, lto, len);

#pragma omp parallel for
  for (cvec_uint i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}




cvec_float * cvec_apply(cvec_float* in, cvec_uint len, cvec_float (*f)(cvec_float))
{
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}



cvec_float cvec_add(cvec_float v1, cvec_float v2) { return v1 + v2; }
cvec_float cvec_subtract(cvec_float v1, cvec_float v2) { return v1 - v2; }
cvec_float cvec_multiply(cvec_float v1, cvec_float v2) { return v1 * v2; }
cvec_float cvec_divide(cvec_float v1, cvec_float v2) { return v1 / v2; }
cvec_float cvec_exp(cvec_float v1, cvec_float v2) { return pow(v1, v2); }




cvec_float *cvec_apply2(cvec_float *in1, cvec_float *in2, cvec_uint len, cvec_float (*f)(cvec_float, cvec_float))
{
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = f(in1[i], in2[i]);
  }
  return rv;
}




cvec_float *cvec_zeros(cvec_uint len)
{
  cvec_float *rv = malloc(len*sizeof(cvec_float));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = 0.0;
  }
  return rv;
}



cvec_float *cvec_copy(cvec_float *source, cvec_uint len)
{
  cvec_float *rv = malloc(sizeof(cvec_float)*len);
#pragma omp parallel for
  for(cvec_uint i = 0; i < len; i++) {
    rv[i] = source[i];
  }
  return rv;
}



cvec_float *cvec_cat(cvec_float *source, cvec_uint len, cvec_float *add, cvec_uint addlen)
{
  cvec_float *rv = malloc(sizeof(cvec_float)*(len+addlen));

#pragma omp parallel for
  for(cvec_uint i = 0; i < len; i++) {
    rv[i] = source[i];
  }

#pragma omp parallel for
  for(cvec_uint i = len; i < (len+addlen); i++) {
    rv[i] = add[i-len];
  }

  return rv;
}




cvec_float *cvec_diff(cvec_float* x, cvec_uint len)
{
  cvec_float *rv = malloc(sizeof(cvec_float)*(len-1));
  cvec_uint i, j;
#pragma omp parallel for
  for (i=0; i < (len-1); i++) {
    j = i+1;
    rv[i] = x[j] - x[i];
  }
  return rv;
}



cvec_float *cvec_slice(cvec_float *x, cvec_uint len, cvec_uint start, cvec_uint stop, cvec_uint skip)
{
  if (stop > len) {
    fprintf(stderr, "\033[31mFATAL ERROR!\033[0m cvec_slice: stop cannot exceed len.\n");
    exit(1);
  }
  if (skip == 0) {
    fprintf(stderr, "\033[31mFATAL ERROR!\033[0m cvec_slice: skip cannot be less than 1 (if you don't want to miss any values, set skip to 1).\n");
    exit(1);
  }
  cvec_uint olen = (stop - start) / skip;
  cvec_float *rv = malloc(sizeof(cvec_float)*olen);
  for (cvec_uint i = start, j = 0; i < stop; i += skip, j++) {
    rv[j] = x[i];
  }
  return rv;
}




cvec_float cvec_get_sumse(cvec_float *x, cvec_float *y, cvec_uint len, cvec_float *coefs, cvec_uint ncoefs)
{
  cvec_float calc_se(cvec_float o, cvec_float g) { 
    return pow(o - g, 2.0); 
  }
  cvec_float calcfit(cvec_float xi) { 
    cvec_float rv = 0.0; 
    for (cvec_uint i = 0; i < ncoefs; i++) 
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




cvec_float *cvec_polyfit(cvec_float *X, cvec_float *Y, cvec_uint len, cvec_uint degree)
{
  // https://github.com/natedomin/polyfit
  cvec_uint maxdeg = 5;
  
  cvec_float *B = calloc(maxdeg+1, sizeof(cvec_float));
  cvec_float *P = calloc(((maxdeg+1)*2)+1, sizeof(cvec_float));
  cvec_float *A = calloc((maxdeg+1)*2*(maxdeg+1), sizeof(cvec_float));

  double x, y, powx;

  cvec_uint i, j, k;

  if (len <= degree || degree > maxdeg)
      return NULL;

  // Identify the column vector
  for (i = 0; i < len; i++) {
    x    = X[i];
    y    = Y[i];
    powx = 1.0;

    for (j = 0; j < (degree + 1); j++) {
      B[j] = B[j] + (y * powx);
      powx  = powx * x;
    }
  }

  // Initialize the PowX array
  P[0] = len;

  // Compute the sum of the Powers of X
  for (i = 0; i < len; i++) {
    x    = X[i];
    powx = X[i];

    for (j = 1; j < ((2 * (degree + 1)) + 1); j++) {
      P[j] = P[j] + powx;
      powx  = powx * x;
    }
  }

  // Initialize the reduction matrix
  //
  for (i = 0; i < (degree + 1); i++) {
    for (j = 0; j < (degree + 1); j++) {
      A[(i * (2 * (degree + 1))) + j] = P[i+j];
    }

    A[(i*(2 * (degree + 1))) + (i + (degree + 1))] = 1;
  }

  // Move the Identity matrix portion of the redux matrix
  // to the left side (find the inverse of the left side
  // of the redux matrix
  for (i = 0; i < (degree + 1); i++) {
    x = A[(i * (2 * (degree + 1))) + i];
    if (x != 0) {
      for (k = 0; k < (2 * (degree + 1)); k++) {
        A[(i * (2 * (degree + 1))) + k] = A[(i * (2 * (degree + 1))) + k] / x;
      }

      for (j = 0; j < (degree + 1); j++) {
        if ((j - i) != 0) {
          y = A[(j * (2 * (degree + 1))) + i];
          for (k = 0; k < (2 * (degree + 1)); k++) {
            A[(j * (2 * (degree + 1))) + k] = A[(j * (2 * (degree + 1))) + k] - y * A[(i * (2 * (degree + 1))) + k];
          }
        }
      }
    }
    else
    {
        // Cannot work with singular matrices
        return NULL;
    }
  }
  
  cvec_float *coefficients = calloc(degree+1, sizeof(cvec_float));
  // Calculate and Identify the coefficients
  for (i = 0; i < (degree + 1); i++) {
    for (j = 0; j < (degree + 1); j++) {
      x = 0;
      for (k = 0; k < (degree + 1); k++) {
        x = x + (A[(i * (2 * (degree + 1))) + (k + (degree + 1))] * B[k]);
      }
      coefficients[i] = x;
    }
  }
  free(A);
  free(B);
  free(P);
  return coefficients;
}



cvec_float *cvec_linearfit(cvec_float *x, cvec_float *y, cvec_uint len)
{
  cvec_float midx = cvec_average(x, len);
  cvec_float midy = cvec_average(y, len);

  cvec_float get_cvec_uint(cvec_float m) { return midy - (m*midx); }

  cvec_float get_sumse(cvec_float *x, cvec_float *y, cvec_uint len, cvec_float gradient) {
    cvec_float cvec_uinterrupt = get_cvec_uint(gradient);
    cvec_float applyfit(cvec_float v) { return cvec_uinterrupt+(gradient*v); }
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
  for (cvec_uint i = 0; i < len+100; i++) {
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
  coefs[0] = get_cvec_uint(m);
  coefs[1] = m;
  return coefs;
}







cvec_float cvec_average(cvec_float* in, cvec_uint len)
{
  cvec_float sum = cvec_sum(in, len);
  return sum / ((cvec_float)len);
}




cvec_float cvec_mean(cvec_float* in, cvec_uint len)
{
  cvec_float sum = cvec_sum(in, len);
  return sum / ((cvec_float)len);
}




cvec_float cvec_median(cvec_float* in, cvec_uint len)
{
  cvec_uint midp = (len%2==0)?(len/2):((len+1)/2);
  cvec_float *sorted = cvec_sort(in, len);
  cvec_float rv = sorted[midp];
  free(sorted);
  return rv;
}




cvec_float cvec_sum(cvec_float* in, cvec_uint len)
{
  cvec_float sum = 0.0;
  for (cvec_uint i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}



cvec_float cvec_interpolate(cvec_float *x, cvec_float *y, cvec_uint len, cvec_float ix)
{
  if (!cvec_in_order(x, len)) {
    fprintf(stderr, "FATAL ERROR! cvec_interpolate: Interpolation expects ordered x.\n");
    exit(1);
  }

  cvec_uint p1 = len, p2 = len;
  if (ix < x[0]) {
    p1 = 0;
    p2 = 1;
  }
  else if (ix > x[len-1]) {
    p1 = len-1;
    p2 = len-2;
  }
  else {
    // find surrounding xs
    for (cvec_uint i = 0, j = 1; j < len; i++, j++) {
      if (x[i] == ix)
        return y[i];
      if (x[j] == ix)
        return y[j];
      if (x[i] < ix && x[j] > ix) {
        p1 = i;
        p2 = j;
      }
    }
    if (p1 == p2) {
      fprintf(stderr, "FATAL ERROR! cvec_interpolate: Could not find points surrounding ix.\n");
      exit(1);
    }
  }


  cvec_float m = (y[p1] - y[p2]) / (x[p1] - x[p2]);
  cvec_float yi = (m * (ix - x[p2])) + y[p2];
  return yi;
}
