#include <stdlib.h>
#include <fftw3.h>
#include <float.h>

#include "cvec.h"

cvec_float *
cvec_fft(cvec_float *in, int in_len) {
  fftw_complex *fft_in, *fft_out;
  fftw_plan plan;

  fft_in = fftw_malloc(sizeof(fftw_complex) * in_len);
  fft_out = fftw_malloc(sizeof(fftw_complex) * in_len);

  for (int i = 0; i < in_len; i++) {
    fft_in[i][0] = in[i];
    fft_in[i][1] = 0.0;
  }

  plan = fftw_plan_dft_1d(in_len, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  
  cvec_float *ffty = malloc(in_len*sizeof(cvec_float));

  for (int i = 0; i < in_len; i++) {
    ffty[i] = fft_out[i][0];
  }


  fftw_destroy_plan(plan);
  fftw_free(fft_in);
  fftw_free(fft_out);
  return ffty;
}

cvec_float *
cvec_freq(int len, cvec_float dt) {
  cvec_float *freq = malloc(len*sizeof(cvec_float));
  double nyq_f = 1/(2.0*dt);
  int hlen = len/2;
  for (int in = 0; in < hlen; in++) {
    cvec_float n = (cvec_float)in;
    freq[hlen + in] = (n*nyq_f) / hlen;
    freq[hlen - in] = -1.0 * ((n*nyq_f) / hlen);
  }
  return freq;
}
