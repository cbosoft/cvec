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
      fprintf(stderr, "FATAL ERROR! gplt_hist: bin width calc has failed.\n");
      exit(1);
    }
    (*output)[bidx] += 1.0;
  }

  (*bins) = malloc(sizeof(cvec_float)*len);
  for (i = 0; i < (*nbins); i++) {
    (*bins)[i] = ((double)i + 0.5)*binw;
  }

}
