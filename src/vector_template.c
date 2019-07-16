#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>



#ifndef CVEC_TYPE
#include "cvec.h"
#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N
#endif

extern int cvec_njobs;




CVEC_TYPE * CVEC_(linspace)(CVEC_TYPE from, CVEC_TYPE to, cvec_uint len)
{
  CVEC_TYPE *rv = malloc(len*sizeof(CVEC_TYPE));
  double tof = (double)to;
  double fromf = (double)from;
  double stepf = ( tof - fromf ) / ( (double)(len - 1) );

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++) {
    double iif = (double)i;
    rv[i] = (CVEC_TYPE)round( (iif * stepf) + fromf );
  }

  return rv;
}




CVEC_TYPE * CVEC_(logspace)(CVEC_TYPE from, CVEC_TYPE to, cvec_uint len)
{
  CVEC_TYPE lfrom = log(from), lto = log(to);
  CVEC_TYPE *rv = CVEC_(linspace)(lfrom, lto, len);

#pragma omp parallel for
  for (cvec_uint i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}




CVEC_TYPE * CVEC_(apply)(
    CVEC_TYPE* in, 
    cvec_uint len, 
    CVEC_TYPE (*f)(CVEC_TYPE))
{
  CVEC_TYPE *rv = malloc(len*sizeof(CVEC_TYPE));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}



CVEC_TYPE CVEC_(add)(CVEC_TYPE v1, CVEC_TYPE v2) { return v1 + v2; }
CVEC_TYPE CVEC_(subtract)(CVEC_TYPE v1, CVEC_TYPE v2) { return v1 - v2; }
CVEC_TYPE CVEC_(multiply)(CVEC_TYPE v1, CVEC_TYPE v2) { return v1 * v2; }
CVEC_TYPE CVEC_(divide)(CVEC_TYPE v1, CVEC_TYPE v2) { return v1 / v2; }
CVEC_TYPE CVEC_(pow(CVEC_TYPE v1, CVEC_TYPE v2) { return pow)(v1, v2); }




CVEC_TYPE * CVEC_(apply2)(
    CVEC_TYPE *in1, 
    CVEC_TYPE *in2, 
    cvec_uint len, 
    CVEC_TYPE (*f)(CVEC_TYPE, CVEC_TYPE))
{
  CVEC_TYPE *rv = malloc(len*sizeof(CVEC_TYPE));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = f(in1[i], in2[i]);
  }
  return rv;
}




CVEC_TYPE * CVEC_(zeros)(cvec_uint len)
{
  CVEC_TYPE *rv = malloc(len*sizeof(CVEC_TYPE));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = 0.0;
  }
  return rv;
}




CVEC_TYPE *CVEC_(ones)(cvec_uint len)
{
  CVEC_TYPE *rv = malloc(len*sizeof(CVEC_TYPE));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = 1.0;
  }
  return rv;
}



CVEC_TYPE *CVEC_(copy)(CVEC_TYPE *source, cvec_uint len)
{
  CVEC_TYPE *rv = malloc(sizeof(CVEC_TYPE)*len);

#pragma omp parallel for
  for(cvec_uint i = 0; i < len; i++) {
    rv[i] = source[i];
  }
  return rv;
}



CVEC_TYPE *CVEC_(cat)(
    CVEC_TYPE *source, 
    cvec_uint len, 
    CVEC_TYPE *add, 
    cvec_uint addlen)
{
  CVEC_TYPE *rv = malloc(sizeof(CVEC_TYPE)*(len+addlen));

#pragma omp parallel for
  for(cvec_uint i = 0; i < len; i++) {
    rv[i] = source[i];
  }

#pragma omp parallel for
  for(cvec_uint i = len; i < len+addlen; i++) {
    rv[i] = add[i-len];
  }

  return rv;
}




CVEC_TYPE *CVEC_(diff)(CVEC_TYPE* x, cvec_uint len)
{
  CVEC_TYPE *rv = malloc(sizeof(CVEC_TYPE)*(len-1));
  cvec_uint i, j;
#pragma omp parallel for
  for (i=0; i < (len-1); i++) {
    j = i+1;
    rv[i] = x[j] - x[i];
  }
  return rv;
}



CVEC_TYPE *CVEC_(slice)(
    CVEC_TYPE *x, 
    cvec_uint len, 
    cvec_uint start, 
    cvec_uint stop, 
    cvec_uint skip)
{
  if (stop > len) 
    cvec_ferr("cvec_slice","stop cannot exceed len");
  
  if (skip == 0)
    cvec_ferr(
        "cvec_slice", 
        "cvec should not be less than one"
        "if you want every value, set skip to 1.");

  cvec_uint olen = (stop - start) / skip;
  CVEC_TYPE *rv = malloc(sizeof(CVEC_TYPE)*olen);
  for (cvec_uint i = start, j = 0; i < stop; i += skip, j++) {
    rv[j] = x[i];
  }
  return rv;
}




CVEC_TYPE CVEC_(get_fit_sumse)(
    CVEC_TYPE *x, 
    CVEC_TYPE *y, 
    cvec_uint len, 
    CVEC_TYPE *coefs, 
    cvec_uint ncoefs)
{
  CVEC_TYPE calc_se(CVEC_TYPE o, CVEC_TYPE g) { 
    return pow(o - g, 2.0); 
  }
  CVEC_TYPE calcfit(CVEC_TYPE xi) { 
    CVEC_TYPE rv = 0.0; 
    for (cvec_uint i = 0; i < ncoefs; i++) 
      rv += coefs[i] * pow(xi, (double)i); 
    return rv; 
  }
  CVEC_TYPE *t = CVEC_(apply)(x, len, &calcfit);
  CVEC_TYPE *se = CVEC_(apply2)(y, t, len, &calc_se);
  CVEC_TYPE sumse = CVEC_(sum)(se, len);
  free(t);
  free(se);
  return sumse;
}




CVEC_TYPE *CVEC_(polyfit)(
    CVEC_TYPE *X, 
    CVEC_TYPE *Y, 
    cvec_uint len, 
    cvec_uint degree)
{
  // https://github.com/natedomin/polyfit
  cvec_uint maxdeg = 5;
  
  CVEC_TYPE *B = calloc(maxdeg+1, sizeof(CVEC_TYPE));
  CVEC_TYPE *P = calloc(((maxdeg+1)*2)+1, sizeof(CVEC_TYPE));
  CVEC_TYPE *A = calloc((maxdeg+1)*2*(maxdeg+1), sizeof(CVEC_TYPE));

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
  
  CVEC_TYPE *coefficients = calloc(degree+1, sizeof(CVEC_TYPE));
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



CVEC_TYPE *CVEC_(linearfit)(CVEC_TYPE *x, CVEC_TYPE *y, cvec_uint len)
{
  CVEC_TYPE midx = CVEC_(average)(x, len);
  CVEC_TYPE midy = CVEC_(average)(y, len);

  CVEC_TYPE get_cvec_uint(CVEC_TYPE m) { return midy - m*midx; }

  CVEC_TYPE get_sumse(
      CVEC_TYPE *x, 
      CVEC_TYPE *y, 
      cvec_uint len, 
      CVEC_TYPE gradient) {
    CVEC_TYPE cvec_uinterrupt = get_cvec_uint(gradient);
    CVEC_TYPE applyfit(CVEC_TYPE v) { return cvec_uinterrupt+(gradient*v); }
    CVEC_TYPE *fity = CVEC_(apply)(x, len, &applyfit);
    CVEC_TYPE getse(CVEC_TYPE y1, CVEC_TYPE y2) {
      return pow(y1 - y2, 2.0);
    }
    CVEC_TYPE *se = CVEC_(apply2)(y, fity, len, &getse);
    CVEC_TYPE sumse = CVEC_(sum)(se, len);
    free(se);
    free(fity);
    return sumse;
  }

  CVEC_TYPE change = 0.1, m = 1.0;
  for (cvec_uint i = 0; i < len+100; i++) {
    CVEC_TYPE err1 = get_sumse(x, y, len, m);
    m += change;
    CVEC_TYPE err2 = get_sumse(x, y, len, m);
    if (err2 > err1) {
      m -= change;
      change *= -0.8;
    }
    else {
      change *= 1.2;
    }
  }
  CVEC_TYPE *coefs = malloc(sizeof(CVEC_TYPE)*2);
  coefs[0] = get_cvec_uint(m);
  coefs[1] = m;
  return coefs;
}



CVEC_TYPE CVEC_(interpolate)(
    CVEC_TYPE *x, 
    CVEC_TYPE *y, 
    cvec_uint len, 
    CVEC_TYPE ix)
{
  if (!CVEC_(in_order)(x, len)) 
    cvec_ferr("svec_interpolate", "Interpolation expects ordered x.");

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
    if (p1 == p2) 
      cvec_ferr("cvec_interpolate", "could not find points surrounding ix");
  }


  CVEC_TYPE m = (y[p1] - y[p2]) / (x[p1] - x[p2]);
  CVEC_TYPE yi = (m * (ix - x[p2])) + y[p2];
  return yi;
}



CVEC_TYPE *CVEC_(rearrange)(
    CVEC_TYPE *x, 
    cvec_uint len,
    cvec_uint *arrangement, 
    cvec_uint alen)
{
  CVEC_TYPE *rv = CVEC_(ones)(alen);

  for (cvec_uint i = 0; i < alen; i++) {
    cvec_uint j = arrangement[i];

    if (j >= len)
      cvec_ferr(
          "cvec_rearrange", 
          "new arrangement index outside of vector length (%u > %u)", 
          j, len);

    rv[i] = x[j];
  }

  return rv;
}


typedef struct sc_td_t {
  cvec_uint from;
  cvec_uint to;
  CVEC_TYPE *x;
  CVEC_TYPE val;
} sc_td_t;

void *CVEC_(setconstthread)(void *vtd) 
{
  sc_td_t *td = (sc_td_t*)vtd;
  for (cvec_uint i = td->from; i < td->to; i++) td->x[i] = td->val;
  return NULL;
}

void CVEC_(set_constant)(CVEC_TYPE *x, cvec_uint len, CVEC_TYPE v)
{
  pthread_t threads[cvec_njobs];
  sc_td_t data[cvec_njobs];
  cvec_uint each = len/cvec_njobs;

  for (cvec_uint i = 0; i < cvec_njobs; i++) {
    data[i].from = i*each;
    data[i].to = (i == cvec_njobs-1) ? (len) : ((i+1)*each);
    data[i].x = x;
    data[i].val = v;

    pthread_create(&threads[i], NULL, CVEC_(setconstthread), &data[i]);
  }

  for (cvec_uint i = 0; i < cvec_njobs; i++) {
    pthread_join(threads[i], NULL);
  }
}




CVEC_TYPE CVEC_(average)(CVEC_TYPE * in, cvec_uint len)
{
  return CVEC_(mean)(in, len);
}




CVEC_TYPE CVEC_(mean)(CVEC_TYPE * in, cvec_uint len)
{
  CVEC_TYPE sum = CVEC_(sum)(in, len);
  return sum / ((CVEC_TYPE)len);
}




CVEC_TYPE CVEC_(median)(CVEC_TYPE * in, cvec_uint len)
{
  cvec_uint midp = (len%2==0)?(len/2):((len+1)/2);
  CVEC_TYPE *sorted;
  CVEC_(sort)(in, len, NULL, &sorted);
  CVEC_TYPE rv = sorted[midp];
  free(sorted);
  return rv;
}



typedef struct sum_td_t {
  cvec_uint from;
  cvec_uint to;
  CVEC_TYPE *v;
  CVEC_TYPE res;
} sum_td_t;

void *CVEC_(sumthread)(void *vtd)
{
  sum_td_t *td = (sum_td_t *)vtd;

  for (cvec_uint i = td->from; i < td->to; i++) {
    td->res += td->v[i];
  }

  return NULL;
}

CVEC_TYPE CVEC_(sum)(CVEC_TYPE * in, cvec_uint len)
{

  sum_td_t data[cvec_njobs];
  pthread_t threads[cvec_njobs];
  cvec_uint each = len/cvec_njobs;

  for (cvec_uint i = 0; i < cvec_njobs; i++) {
    data[i].from = i*each;
    data[i].to = (i == cvec_njobs-1) ? (len) : ((i+1)*each);
    data[i].v = in;
    data[i].res = 0.0;

    pthread_create(&threads[i], NULL, CVEC_(sumthread), &data[i]);
  }

  for (cvec_uint i = 0; i < cvec_njobs; i++) {
    pthread_join(threads[i], NULL);
  }

  CVEC_TYPE sum = 0.0;
  for (cvec_uint i = 0; i < cvec_njobs; i++) {
    sum += data[i].res;
  }

  return sum;
}




CVEC_TYPE CVEC_(prod)(CVEC_TYPE * in, cvec_uint len)
{
  CVEC_TYPE prod = 1.0;
  for (cvec_uint i = 0; i < len; i++) {
    prod *= in[i];
  }
  return prod;
}
