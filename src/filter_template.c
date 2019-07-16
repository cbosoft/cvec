#ifndef CVEC_TYPE

#include <stdlib.h>
#include <stdio.h>
#include "cvec.h"

#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N

#endif




CVEC_TYPE *CVEC_(moving_average)(CVEC_TYPE *x, cvec_uint len, cvec_uint w, CVEC_TYPE (*avfunc)(CVEC_TYPE *x, cvec_uint len))
{
  if (w == 0)
    return CVEC_(copy)(x, len);

  if (w < 3)
    cvec_ferr("cvec_moving_average", "window cannot be smaller than 3 (is %u).", w);

  CVEC_TYPE *res = malloc(len*sizeof(CVEC_TYPE));
  int hw = (w%2 == 0)?(w/2):((w+1/2));
  
  if (w >= len) 
    cvec_ferr("cvec_moving_average", "window cannot be greater than the length of the vector.");

  if (avfunc == NULL)
    avfunc = &cvec_average;

  for (cvec_uint i = 0; i < len; i++) {
    cvec_uint before = 0;
    cvec_uint after = i + hw;

    if (hw > i)
      after += hw - i;
    else
      before = i - hw;

    if (after >= len) {
      before -= (after - len);
      after = len-1;
    }
    CVEC_TYPE *window = CVEC_(slice)(x, len, before, after, 1);
    // for (int i = 0; i < after-before; i++)
    //   fprintf(stderr, "%f, ", window[i]);
    // fprintf(stderr, "\n\n");
    res[i] = avfunc(window, after-before);
    free(window);
  }

  return res;
}

