#include <stdio.h>
#include <cvec.h>

int main(void)
{
  cvec_uint n = 100;

  cvec_float * vf = cvec_linspace(0.0, 100.0, 100);
  cvec_int * vi = cvec_int_linspace(0, 100, 100);

  for (cvec_uint i = 0; i < n; i++) {
    fprintf(stderr, "%f, %d\n", vf[i], vi[i]);
  }

  return 0;
}
