#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "cvec.h"

void
cvec_hist(cvec_float *input, cvec_uint len, cvec_float **output, cvec_float **bins, cvec_uint *nbins) {

  if ((*nbins) <= 0)
    (*nbins) = (cvec_uint)pow((cvec_float)len, 0.5);

  cvec_float binw, min = CVEC_FLOAT_MAX, max = CVEC_FLOAT_MIN;
  for (cvec_uint i = 0; i < len; i++) {
    if (input[i] < min)
      min = input[i];
    if (input[i] > max)
      max = input[i];
  }
  
  // are they equalish?
  if ((max - min) < 0.00001525878906) {
    min = max - 2.0;
    max = max + 2.0;
  }

  binw = (max - min) / ((cvec_float) (*nbins));

  (*output) = cvec_zeros((*nbins));

  for (cvec_uint i = 0; i < len; i++) {
    cvec_uint bidx = (cvec_uint)(input[i] / binw);
    if (bidx > (*nbins) || bidx < 0) {
      fprintf(stderr, "FATAL ERROR! cvec_hist: bin width calc has failed.\n");
      exit(1);
    }
    (*output)[bidx] += 1.0;
  }

  (*bins) = malloc(sizeof(cvec_float)*len);
  for (cvec_uint i = 0; i < (*nbins); i++) {
    (*bins)[i] = ((cvec_float)i + 0.5)*binw;
  }

}




void
cvec_autocorr(cvec_float* x, cvec_float *y, int len, cvec_float **res_x, cvec_float **res_y, int *nbins) {
  // time correlation
  //
  // for each data point, for each lag dt, get product, sum:
  //    tcorr = VxV(l) = int _-inf^inf V(t)xV(t+l) dt
  // normalise wrt initial

  // for discrete signals, autocorr R is given by:
  // R = sum_0^N y(n) av(y(n-l))

  if (x == NULL || y == NULL)
    cvec_ferr("cvec_autocorr","autocorrelation input cannot be NULL");

  if (!cvec_in_order(x, len))
    cvec_ferr("cvec_autocorr", "autocorrelation input must be in time-order (i.e. sorted by x)");

  cvec_float *dx = cvec_diff(x, len);
  cvec_float mindt = CVEC_FLOAT_MAX, maxdt = -CVEC_FLOAT_MAX;

  for (cvec_uint i = 0; i < (len-1); i++) {
    if (dx[i] < mindt)
      mindt = dx[i];
    if (dx[i] > maxdt)
      maxdt = dx[i];
  }
  cvec_float binw = mindt * 1.0;
  free(dx);

  (*nbins) = ceil((x[len-1] - x[0]) / binw);

  (*res_x) = malloc(sizeof(cvec_float)*(*nbins));
  (*res_y) = cvec_zeros((*nbins));

  // get correlation
  for (cvec_uint i = 0; i < len; i++) {
    for (cvec_uint j = i+1; j < len; j++) {
      cvec_float dt = x[j] - x[i];
      cvec_uint ibin = (cvec_uint)(dt/binw);
      if (ibin >= (*nbins)) {
        cvec_warn(
            "cvec_autocorr", 
            "overspill in bin index calculation (%u > %d)", 
            ibin, (*nbins));
        continue;
      }
      (*res_y)[ibin] += y[i]*y[j];
    }
  }

  // fill res_x
  for (cvec_uint i = 0; i < (*nbins); i++) {
    (*res_x)[i] = ((cvec_float)(i + 1)) * binw;
  }

  // get max and normalize output
  cvec_float maxv = cvec_max((*res_y), (*nbins));
  cvec_float divmax(cvec_float v) { return v / maxv; }
  cvec_apply((*res_y), (*nbins), &divmax);

}



cvec_float
cvec_std(cvec_float *x, cvec_uint len)
{
  cvec_float average = cvec_average(x, len);
  cvec_float getsquaredev(cvec_float v) { return pow(average-v,2.0); }
  cvec_float *square_deviation = cvec_apply(x, len, &getsquaredev);
  cvec_float sum_square_deviation = cvec_sum(square_deviation, len);
  cvec_float standard_deviation = sum_square_deviation / ((cvec_float)len);
  standard_deviation = pow(standard_deviation, 0.5);
  free(square_deviation);
  return standard_deviation;
}
