#ifndef CVEC_TYPE
#include <stdlib.h>
#include "cvec.h"
#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N
#endif




void CVEC_(get_limit)(
    CVEC_TYPE *in, 
    cvec_uint len, 
    cvec_int flags, 
    CVEC_TYPE threshold, 
    CVEC_TYPE *limit, 
    cvec_uint *index)
{
  CVEC_TYPE dircoef = (FLAGSET(flags,CVEC_LIMITS_MINIMUM)) ? -1.0 : 1.0;

  CVEC_TYPE limval = (FLAGSET(flags,CVEC_LIMITS_MINIMUM)) ? CVEC_FLOAT_MAX : CVEC_FLOAT_MAX;
  cvec_uint limind = 0;

  cvec_uint use_thresh = FLAGSET(flags,CVEC_LIMITS_THRESHOLD);
  CVEC_TYPE dirthresh = dircoef*threshold;

  for (cvec_uint i = 0; i < len ; i++) {
    if (dircoef*in[i] > dircoef*limval && ((dircoef*in[i] > dirthresh) || (!use_thresh) ) ) {
      limval = in[i];
      limind = i;
    }
  }

  (*limit) = limval;

  if (index != NULL)
    (*index) = limind;
}




CVEC_TYPE CVEC_(min)(CVEC_TYPE *in, cvec_uint len)
{ 
  CVEC_TYPE rv; 
  CVEC_(get_limit)(in, len, CVEC_LIMITS_MINIMUM, 0.0, &rv, NULL); 
  return rv; 
}




CVEC_TYPE CVEC_(max)(CVEC_TYPE *in, cvec_uint len)
{ 
  CVEC_TYPE rv; 
  CVEC_(get_limit)(in, len, CVEC_LIMITS_MAXIMUM, 0.0, &rv, NULL); 
  return rv; 
}
