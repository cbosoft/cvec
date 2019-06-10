#include <stdio.h>
#include <cvec.h>
#include <stdlib.h>

void
test_matrix(cvec_float **M, cvec_uint R, cvec_uint C) {
  fprintf(stderr, "%ux%x matrix:\n", R, C);
  cvec_print_matrix(M, R, C);
  cvec_float det = cvec_matrix_determinant(M, R, C);
  fprintf(stderr, "Determinant: %f\n", det);
  cvec_float **inv_M = cvec_matrix_invert(M, R, C);

  if (inv_M != NULL) {
    fprintf(stderr, "Inverse:\n");
    cvec_print_matrix(inv_M, R, C);
    cvec_matrix_free(inv_M, R, C);
  }

  fprintf(stderr, "\n");
}

int
main (int argc, const char** argv)
{
  int R = 5, C = 5;
  srand(1);
  cvec_float **M = cvec_matrix_new(R, C, &cvec_matgen_identity);
  cvec_float **N = cvec_matrix_new(R, C, &cvec_matgen_random);
  cvec_float **O = cvec_matrix_new(R, C, &cvec_matgen_ones);

  test_matrix(M, R, C);
  test_matrix(N, R, C);
  test_matrix(O, R, C);

  cvec_matrix_free(M, R, C);
  cvec_matrix_free(N, R, C);
  cvec_matrix_free(O, R, C);
}
