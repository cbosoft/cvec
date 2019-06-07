#include <stdlib.h>
#include <stdio.h>
#include "cvec.h"

cvec_float *
cvec_moving_average(cvec_float *x, cvec_uint len, cvec_uint w)
{
  cvec_float *res = malloc(len*sizeof(cvec_float));
  int hw = (w%2 == 0)?(w/2):((w+1/2));
  
  if (w >= len) {
    fprintf(stderr, "FATAL ERROR! cvec_moving_average: moving average window cannot be greater than the length of the vector.\n");
    exit(1);
  }

  for (cvec_uint i = 0; i < len; i++) {
    cvec_uint before = 0;
    cvec_uint after = i + hw;
    if (hw > i) {
      after += hw - i;
    }
    else {
      before = i - hw;
    }
    if (after >= len) {
      before -= (after - len);
      after = len-1;
    }
    cvec_float *window = cvec_slice(x, len, before, after, 1);
    res[i] = cvec_average(window, after-before);
    free(window);
  }

  return res;
}
