#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cvec.h>
#include <cgnuplot.h>

int
main (void)
{
  // generate random data
  int len = 1000, ncoefs = 2;
  cvec_float *x = cvec_linspace(0.0, 1.0, len);
  cvec_float *actual_coefs = malloc(ncoefs*sizeof(cvec_float));
  actual_coefs[0] = 2.0;
  actual_coefs[1] = 10.0;
  cvec_float gety(cvec_float x) { return actual_coefs[0] + actual_coefs[1]*x; }
  cvec_float *y = cvec_apply(x, len, &gety);
  cvec_float addnoise(cvec_float y) { return (0.9 + (((double)(rand() % 20)) / 100.0)) * y; }
  cvec_float *n = cvec_apply(y, len, &addnoise);

  cvec_float *simple_fit_coefs = cvec_linearfit(x, y, len);
  cvec_float *fit_coefs = cvec_linearfit(x, n, len);

  for (int i = 0; i < ncoefs; i++) {
    fprintf(stderr, "%f %f %f\n", actual_coefs[i], simple_fit_coefs[i], fit_coefs[i]);
  }
  
  cvec_float *coefs = simple_fit_coefs;
  cvec_float applyfit(cvec_float v) { return (coefs[1] *v)+coefs[0]; }
  cvec_float *fy = cvec_apply(x, len, &applyfit);
  coefs = fit_coefs;
  cvec_float *fn = cvec_apply(x, len, &applyfit);

  gplt_ctrl *h = gplt_init();
  gplt_plot_xy(h, x, y, len, "y");
  gplt_plot_xy(h, x, fy, len, "simple_fit(x)");
  gplt_plot_xy(h, x, fn, len, "fit(x)");
  gplt_set_style(h, "points");
  gplt_plot_xy(h, x, n, len, "noisey y");
  gplt_draw(h);
  for (int i = 0; i < 100; i++) sleep(1);
  gplt_close(h);
  return 0;
}
