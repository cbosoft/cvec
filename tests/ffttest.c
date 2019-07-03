#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "../src/cvec.h"

#define N 4

int
main(int argc, const char **argv) {
  
  int len = 210;
  cvec_float as[N] = {5.0, 10.0, 50.0, 100.0}, pi=3.1415926;


  cvec_float *angle = cvec_linspace(0.0, 2.0*pi, len);
  cvec_float *total_val = cvec_zeros(len);

  fprintf(stderr, "Creating composite signal of %d sine waves. Frequencies:\n", N);
  for (int i = 0; i < N; i++) {
    fprintf(stderr, "  - %.1f Hz\n", as[i]);
    cvec_float f(cvec_float angle) {
      return sin(as[i]*angle);
    }
    cvec_float *val = cvec_apply(angle, len, &f);
    for (int i = 0; i < len; i++) {
      total_val[i] += val[i];
    }
    free(val);
    
  }
  free(angle);
  
  cvec_float *long_ffty = cvec_fft(total_val, len);
  cvec_float *long_fftx = cvec_freq(len, 1.0/len);
  cvec_float *cvec_slice(cvec_float *source, cvec_uint len, cvec_uint start, cvec_uint stop, cvec_uint skip);

  cvec_float *ffty = cvec_slice(long_ffty, len, 1, len/2, 1);
  cvec_float *fftx = cvec_slice(long_fftx, len, 1, len/2, 1);
  len = len/2;

  
  fprintf(stderr, "Plotting fourier transform of composite signal. Expect \n"
                  "to have peaks at each of the five frequencies, with power \n"
                  "increasing with peak frequency.\n\n");

  cvec_float *peaks;
  cvec_uint *peak_indices;
  cvec_uint npeaks;
  cvec_peakdet(ffty, len, 0.0, &peaks, &peak_indices, &npeaks, CVEC_PEAKS_POSITIVE);

  int rv = 0;

  if (npeaks < N) {
    fprintf(stderr, "Test failed: FFT missed frequencies (got %d, expected %d).\n", npeaks, N);
    rv = 1;
  }
  if (npeaks > N) {
    fprintf(stderr, "Test failed: FFT found too many frequencies (got %d, expected %d).\n", npeaks, N);
    rv = 1;
  }

  
  for (cvec_uint i = 0; i < N; i++) {
    cvec_float got = fftx[peak_indices[i]];
    cvec_float expected = as[N-i-1];
    
    cvec_float diff = got - expected;
    cvec_float magdiff = pow(pow(diff, 2.0), 0.5);

    if (magdiff > 0.01) {
      fprintf(stderr, "Test failed: FFT incorrectly identified frequencies (got %f, expected %f).\n", got, expected);
      rv = 1;
    }
    else
      fprintf(stderr, "FFT correctly identified frequency %.0f.\n", got);

  }
  

  return rv;
}
