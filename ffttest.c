#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cvec.h>
#include <unistd.h>
#include <cgnuplot.h>

int
main(int argc, const char **argv) {
  
  int len = 10000;
  cvec_float as[5] = {1.0, 3.0, 5.0, 100.0}, pi=3.1415926;


  cvec_float *angle = cvec_linspace(0.0, 2.0*pi, len);
  cvec_float *total_val = cvec_zeros(len);
  
  gplt_ctrl *h = gplt_init();
  gplt_cmd(h, "set terminal qt");
  
  for (int i = 0; i < 5; i++) {
    cvec_float f(cvec_float angle) {
      return sin(as[i]*angle);
    }
    cvec_float *val = cvec_apply(angle, len, &f);
    for (int i = 0; i < len; i++) {
      total_val[i] += val[i];
    }
    free(val);
  }

  cvec_float *ffty = cvec_fft(total_val, len);
  cvec_float *fftx = cvec_freq(len, 1.0/len);
  
  gplt_plot_xy(h, fftx, ffty, len, NULL);

  gplt_draw(h);

  for (int i = 0; i < 60; i++) {
    sleep(1);
  }

  gplt_close(h);
  free(angle);

  return 0;
}
