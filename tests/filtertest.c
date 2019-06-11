#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cvec.h>
#include <cgnuplot.h>


int 
main (void) 
{
  int len = 1000, ncoefs = 2;
  cvec_float *x = cvec_linspace(0.0, 1.0, len);
  
  cvec_float *actual_coefs = malloc(ncoefs*sizeof(cvec_float));
  actual_coefs[0] = 2.0;
  actual_coefs[1] = 10.0;

  cvec_float gety(cvec_float x) { return actual_coefs[0] + actual_coefs[1]*x; }
  cvec_float *y = cvec_apply(x, len, &gety);
  free(actual_coefs);

  cvec_float addnoise(cvec_float y) { return (0.9 + (((double)(rand() % 20)) / 100.0)) * y; }
  cvec_float *n = cvec_apply(y, len, &addnoise);

  cvec_float *av = cvec_moving_average(n, len, 10, NULL);

  gplt_ctrl *h = gplt_init();

  gplt_plot_xy(h, x, n, len, NULL);
  gplt_plot_xy(h, x, av, len, NULL);

  gplt_draw(h);


  gplt_waitkey();

  gplt_close(h);

  free(av);
  free(n);
  free(y);
  free(x);

}
