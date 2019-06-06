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
  cvec_float *rv = malloc(sizeof(cvec_float)*len);
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
cvec_polyfit(cvec_float *X, cvec_float *Y, int len, int degree)
{
  // https://github.com/natedomin/polyfit
  int maxdeg = 5;
  
  // cvec_float B[maxOrder+1] = {0.0f};
  // cvec_float P[((maxOrder+1) * 2)+1] = {0.0f};
  // cvec_float A[(maxOrder + 1)*2*(maxOrder + 1)] = {0.0f};
  cvec_float *B = calloc(maxdeg+1, sizeof(cvec_float));
  cvec_float *P = calloc(((maxdeg+1)*2)+1, sizeof(cvec_float));
  cvec_float *A = calloc((maxdeg+1)*2*(maxdeg+1), sizeof(cvec_float));

  double x, y, powx;

  int i, j, k;

  // Verify initial conditions....
  // ----------------------------------

  // This method requires that the countOfElements > 
  // (degree+1) 
  if (len <= degree)
      return NULL;

  // This method has imposed an arbitrary bound of
  // degree <= maxOrder.  Increase maxOrder if necessary.
  if (degree > maxdeg)
      return NULL;

  // Begin Code...
  // ----------------------------------

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
