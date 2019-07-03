#include <stdlib.h>

#include "cvec.h"

void cvec_get_limit(cvec_float *in, cvec_uint len, cvec_int flags, cvec_float threshold, cvec_float *limit, cvec_uint *index)
{
  cvec_float dircoef = (FLAGSET(flags,CVEC_LIMITS_MINIMUM)) ? -1.0 : 1.0;

  cvec_float limval = (FLAGSET(flags,CVEC_LIMITS_MINIMUM)) ? CVEC_FLOAT_MAX : CVEC_FLOAT_MAX;
  cvec_uint limind = 0;

  cvec_uint use_thresh = FLAGSET(flags,CVEC_LIMITS_THRESHOLD);
  cvec_float dirthresh = dircoef*threshold;

  for (cvec_uint i = 0; i < len ; i++) {
    if (dircoef*in[i] > dircoef*limval && ((dircoef*in[i] > dirthresh) || (!use_thresh) ) ) {
      limval = in[i];
      limind = i;
    }
  }

  (*limit) = limval;
  (*index) = limind;
}

cvec_float cvec_min(cvec_float *in, cvec_uint len)
{ 
  cvec_float rv; 
  cvec_get_limit(in, len, CVEC_LIMITS_MINIMUM, 0.0, &rv, NULL); 
  return rv; 
}

cvec_float cvec_max(cvec_float *in, cvec_uint len)
{ 
  cvec_float rv; 
  cvec_get_limit(in, len, CVEC_LIMITS_MAXIMUM, 0.0, &rv, NULL); 
  return rv; 
}
