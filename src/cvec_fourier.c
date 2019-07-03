#include <stdlib.h>
#include <fftw3.h>
#include <float.h>

#include "cvec.h"

cvec_float *cvec_fft(cvec_float *in, cvec_uint in_len)
{
  fftw_complex *fft_in, *fft_out;
  fftw_plan plan;

  fft_in = fftw_malloc(sizeof(fftw_complex) * in_len);
  fft_out = fftw_malloc(sizeof(fftw_complex) * in_len);

  for (cvec_uint i = 0; i < in_len; i++) {
    fft_in[i][0] = in[i];
    fft_in[i][1] = 0.0;
  }

  plan = fftw_plan_dft_1d(in_len, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  
  cvec_float *ffty = malloc(in_len*sizeof(cvec_float));

  for (cvec_uint i = 0; i < in_len; i++) {
    ffty[i] = fft_out[i][0];
  }


  fftw_destroy_plan(plan);
  fftw_free(fft_in);
  fftw_free(fft_out);
  return ffty;
}

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
