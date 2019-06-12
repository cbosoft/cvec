#include <stdlib.h>
#include <stdio.h>
#include "cvec.h"

cvec_float *
cvec_moving_average(cvec_float *x, cvec_uint len, cvec_uint w, cvec_float (*avfunc)(cvec_float *x, cvec_uint len))
{
  if (w == 0)
    return cvec_copy(x, len);

  if (w < 10) {
    fprintf(stderr, "\033[31mFATAL ERROR!\033[0m cvec_moving_average: window cannot be less than 10.\n");
    exit(1);
  }

  cvec_float *res = malloc(len*sizeof(cvec_float));
  int hw = (w%2 == 0)?(w/2):((w+1/2));
  
  if (w >= len) {
    fprintf(stderr, "FATAL ERROR! cvec_moving_average: moving average window cannot be greater than the length of the vector.\n");
    exit(1);
  }

  if (avfunc == NULL) {
    avfunc = &cvec_average;
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
    // for (int i = 0; i < after-before; i++)
    //   fprintf(stderr, "%f, ", window[i]);
    // fprintf(stderr, "\n\n");
    res[i] = avfunc(window, after-before);
    free(window);
  }

  return res;
}
