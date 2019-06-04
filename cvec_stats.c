#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "cvec.h"

void
cvec_hist(cvec_float *input, int len, cvec_float **output, cvec_float **bins, int *nbins) {
  int i;

  if ((*nbins) <= 0)
    (*nbins) = (int)pow((double)len, 0.5);

  double binw, min = DBL_MAX, max = -DBL_MAX;
  for (int i = 0; i < len; i++) {
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

  binw = (max - min) / ((double) (*nbins));

  (*output) = cvec_zeros((*nbins));

  for (i = 0; i < len; i++) {
    int bidx = (int)(input[i] / binw);
    if (bidx > (*nbins) || bidx < 0) {
      fprintf(stderr, "FATAL ERROR! cvec_hist: bin width calc has failed.\n");
      exit(1);
    }
    (*output)[bidx] += 1.0;
  }

  (*bins) = malloc(sizeof(cvec_float)*len);
  for (i = 0; i < (*nbins); i++) {
    (*bins)[i] = ((double)i + 0.5)*binw;
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

  if (x == NULL || y == NULL) {
    fprintf(stderr, "FATAL ERROR! cvec_autocorr: input cannot be NULL\n");
    exit(1);
  }

  cvec_float *dx = cvec_diff(x, len);
  cvec_float mindt = CVEC_FLOAT_MAX, maxdt = -CVEC_FLOAT_MAX;

  for (int i = 0; i < (len-1); i++) {
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
  for (int i = 0; i < len; i++) {
    for (int j = i+1; j < len; j++) {
      cvec_float dt = x[j] - x[i];
      int ibin = (int)(dt/binw);
      (*res_y)[ibin] += y[i]*y[j];
    }
  }

  // fill res_x
  for (int i = 0; i < (*nbins); i++) {
    (*res_x)[i] = ((cvec_float)(i + 1)) * binw;
  }

  // get max and normalize output
  cvec_float maxv = cvec_max((*res_y), (*nbins));
  cvec_float divmax(cvec_float v) { return v / maxv; }
  cvec_apply((*res_y), (*nbins), &divmax);

}
