#include <stdlib.h>
#include <math.h>

#include "cvec.h"




cvec_uint *cvec_uint_linspace(cvec_uint from, cvec_uint to, cvec_uint len)
{
  cvec_uint *rv = malloc(len*sizeof(cvec_uint));
  cvec_uint step = (to - from) / ((cvec_uint)(len - 1));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++) {
    rv[i] = (step*((cvec_uint)i)) + from;
  }

  return rv;
}




cvec_uint *cvec_uint_logspace(cvec_uint from, cvec_uint to, cvec_uint len)
{
  cvec_uint lfrom = log(from), lto = log(to);
  cvec_uint *rv = cvec_uint_linspace(lfrom, lto, len);

#pragma omp parallel for
  for (cvec_uint i  = 0; i < len; i++) {
    rv[i] = pow(10.0, rv[i]);
  }

  return rv;
}



cvec_uint *cvec_uint_apply(cvec_uint* in, cvec_uint len, cvec_uint (*f)(cvec_uint))
{
  cvec_uint *rv = malloc(len*sizeof(cvec_uint));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}




cvec_uint *cvec_uint_zeros(cvec_uint len)
{
  cvec_uint *rv = malloc(len*sizeof(cvec_uint));

#pragma omp parallel for
  for (cvec_uint i = 0; i < len; i++){
    rv[i] = 0;
  }
  return rv;
}




cvec_uint *cvec_uint_copy(cvec_uint * source, cvec_uint len)
{
  cvec_uint *rv = malloc(sizeof(cvec_uint)*len);
#pragma omp parallel for
  for(cvec_uint i = 0; i < len; i++) {
    rv[i] = source[i];
  }
  return rv;
}




cvec_uint *cvec_uint_diff(cvec_uint* x, cvec_uint len)
{
  cvec_uint *rv = malloc(sizeof(cvec_uint)*(len-1));
  cvec_uint i, j;
#pragma omp parallel for
  for (i=0; i < (len-1); i++) {
    j = i+1;
    rv[i] = x[j] - x[i];
  }
  return rv;
}




cvec_uint cvec_uint_max(cvec_uint* x, cvec_uint len)
{
  cvec_uint rv = CVEC_INT_MIN;
  for (cvec_uint i = 0; i < len; i++) {
    if (x[i] > rv) {
      rv = x[i];
    }
  }
  return rv;
}




cvec_uint cvec_uint_min(cvec_uint* x, cvec_uint len)
{
  cvec_uint rv = CVEC_INT_MAX;
  for (cvec_uint i = 0; i < len; i++) {
    if (x[i] < rv) {
      rv = x[i];
    }
  }
  return rv;
}




cvec_uint cvec_uint_average(cvec_uint* in, cvec_uint len)
{
  cvec_uint sum = cvec_uint_sum(in, len);
  return sum / len;
}




cvec_uint cvec_uint_sum(cvec_uint* in, cvec_uint len)
{
  cvec_uint sum = 0;
  for (cvec_uint i = 0; i < len; i++) {
    sum += in[i];
  }
  return sum;
}
