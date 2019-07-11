#include <stdio.h>
#include <stdlib.h>
#include <cvec.h>

int main(void)
{
  cvec_uint n = 100, rv = 0;

  cvec_float * vf = cvec_linspace(0.0, 100.0, 100);
  cvec_int * vi = cvec_int_linspace(0, 100, 100);

  cvec_test_pass("simple vector creation", "vectors created without error");

  if (vf[n-1] != 100.0) {
    rv += 1;
    cvec_test_fail("linear spacing test (float)", "values not inclusive (last value %f, expected 100.0)");
  }
  else
    cvec_test_pass("linear spacing test (float)", "values created with expected range");

  if (vi[n-1] != 100) {
    rv += 1;
    cvec_test_fail("linear spacing test (int)", "values not inclusive (last value %f, expected 100.0)");
  }
  else
    cvec_test_pass("linear spacing test (int)", "values created with expected range");

  // TODO test is actually linear i.e. next val:this val = this val:prev val
  // TODO test log spacing

  free(vf);
  free(vi);

  cvec_test_pass("simple vector destruction", "vectors free'd without error");

  return rv;
}
