#include <stdlib.h>
#include <float.h>

#include "cvec.h"

cvec_float *cvec_freq(cvec_uint len, cvec_float dt)
{
  cvec_float *freq = malloc(len*sizeof(cvec_float));
  cvec_float nyq_f = 1/(2.0*dt);
  cvec_uint hlen = (len%2==0)?len:(len+1); // unsure about this
  hlen /= 2;
  for (cvec_uint in = 0; in < hlen; in++) {
    cvec_float n = (cvec_float)in;
    freq[in] = (n*nyq_f) / hlen;
    freq[len - in - 1] = (-1.0*n*nyq_f) / hlen;
  }
  return freq;
}

void cvec_gaussian_fft(cvec_float* in, cvec_uint in_len, cvec_float **out_y, cvec_float **out_x, cvec_uint *out_len)
{
  cvec_ferr("cvec_gaussian_fft", "not yet implemented!");
}
