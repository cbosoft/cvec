#include <stdlib.h>
#include "cvec.h"

cvec_float *
cvec_moving_average(cvec_float *x, int len, int w)
{
  cvec_float *res = malloc(len*sizeof(cvec_float));
  int hw = (w%2 == 0)?(w/2):((w+1/2));
  
  for (int i = 0; i < len; i++) {
    int before = i - hw;
    int after = i + hw;
    if (before < 0) {
      after -= before;
      before = 0;
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