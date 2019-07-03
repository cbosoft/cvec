#include <stdlib.h>

#include "cvec.h"

cvec_uint get_uncounted_limit(cvec_float *x, cvec_uint len, cvec_int *counted, cvec_int flags, cvec_float threshold)
{
  cvec_float lim = FLAGSET(flags, CVEC_LIMITS_MINIMUM) ? CVEC_FLOAT_MAX : CVEC_FLOAT_MIN;
  cvec_float dircoef = FLAGSET(flags, CVEC_LIMITS_MINIMUM) ? -1.0 : 1.0;
  cvec_uint limat = 0;

  for (cvec_uint i = 0 ; i < len ; i++) {
    if (counted[i])
      continue;

    if (dircoef*x[i] > dircoef*lim && dircoef*x[i] > dircoef*threshold) {
      lim = x[i];
      limat = i;
    }
  }
  
  return limat;
}

void
cvec_peakdet(cvec_float* signal, 
    cvec_uint len, 
    cvec_float threshold, 
    cvec_float **peaks, 
    cvec_uint **peak_indices, 
    cvec_uint *npeaks, 
    cvec_int flags) 
{

  // get minimum in points sequence
  // this is the start of a peak
  // step back until points voltage goes beyond thresh
  // this is the end of the peak

  cvec_float *_peaks = malloc(sizeof(cvec_float)*len); // more than enough space for all peaks
  cvec_uint  *_indcs = malloc(sizeof(cvec_uint)*len);
  cvec_int *counted = cvec_int_zeros(len);

  cvec_uint c = 0;
  cvec_float dircoef = FLAGSET(flags, CVEC_PEAKS_NEGATIVE) ? -1.0 : 1.0;
  cvec_int limflags = FLAGSET(flags, CVEC_PEAKS_NEGATIVE) ? CVEC_LIMITS_MINIMUM : CVEC_LIMITS_MAXIMUM;
  limflags |= CVEC_LIMITS_THRESHOLD;
  cvec_uint mindex;

  do {
    mindex = get_uncounted_limit(signal, len, counted, limflags, threshold);

    for (cvec_uint i = mindex; i > 0; i--) {
      counted[i] = 1;
      if (dircoef*signal[i] < dircoef*threshold)
        break;
    }
    for (cvec_uint i = mindex; i < len; i++) {
      counted[i] = 1;
      if (dircoef*signal[i] < dircoef*threshold)
        break;
    }

    _peaks[c] = signal[mindex];
    _indcs[c] = mindex;
    c ++;
  } while (mindex > 0);

  if (c == 0)
    cvec_ferr("cvec_peakdet", "no peaks found!");

  _peaks = realloc(_peaks, sizeof(cvec_float)*c); // shrink to actual number of peaks

  (*peaks) = _peaks;
  (*peak_indices) = _indcs;
  (*npeaks) = c;

}
