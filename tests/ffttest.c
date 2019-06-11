#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cvec.h>
#include <unistd.h>
#include <cgnuplot.h>

int
main(int argc, const char **argv) {
  
  int len = 210;
  cvec_float as[5] = {3.0, 5.0, 10.0, 50.0, 100.0}, pi=3.1415926;


  cvec_float *angle = cvec_linspace(0.0, 2.0*pi, len);
  cvec_float *total_val = cvec_zeros(len);

  gplt_ctrl *h = gplt_init();
  gplt_cmd(h, "set terminal qt");
  //gplt_cmd(h, "set logscale y");
  gplt_cmd(h, "set xrange [0:110]");
  gplt_cmd(h, "set xlabel \"Frequency f/Hz\"");
  gplt_cmd(h, "set ylabel \"Power\"");
  
  fprintf(stderr, "Creating composite signal of 5 sine waves. Frequencies:\n");
  for (int i = 0; i < 5; i++) {
    fprintf(stderr, "  - %.1f Hz\n", as[i]);
    cvec_float f(cvec_float angle) {
      return sin(as[i]*angle);
    }
    cvec_float *val = cvec_apply(angle, len, &f);
    for (int i = 0; i < len; i++) {
      total_val[i] += val[i];
    }
    free(val);
    
    gplt_cmd(h, "set arrow from %f,-10 to %f,0", as[i], as[i]);
  }
  
  cvec_float *ffty = cvec_fft(total_val, len);
  cvec_float *fftx = cvec_freq(len, 1.0/len);

  
  fprintf(stderr, "Plotting fourier transform of composite signal. Expect \n"
                  "to have peaks at each of the five frequencies, with power \n"
                  "increasing with peak frequency.\n");
  gplt_plot_xy(h, fftx, ffty, len/2, NULL);

  h->plots[0]->lw = 1;

  gplt_draw(h);

  gplt_waitkey();

  gplt_close(h);
  free(angle);

  return 0;
}
