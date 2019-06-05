#include <stdio.h>
#include <cvec.h>

int
main (int argc, const char** argv)
{
  int R = 3, C = 3;
  cvec_float g2(int r, int c) {
    return ((cvec_float)((r+1)+(c+1)));
  }
  cvec_float g1(int r, int c) {
    return ((cvec_float)( (r==c)?(1.0):(0.0)   ));
  }
  cvec_float ** M = cvec_matrix_new(R, C, &g1);
  cvec_float ** N = cvec_matrix_new(R, C, &g2);

  cvec_print_matrix(M, R, C);
  cvec_float det = cvec_matrix_determinant(M, R, C);
  fprintf(stderr, "Determinant: %f\n", det);
  cvec_float **inv_M = cvec_matrix_invert(M, R, C);
  fprintf(stderr, "Inverse:\n");
  cvec_print_matrix(inv_M, R, C);

  cvec_print_matrix(N, R, C);
  det = cvec_matrix_determinant(N, R, C);
  fprintf(stderr, "Determinant: %f\n", det);
  cvec_float **inv_N = cvec_matrix_invert(N, R, C);
  fprintf(stderr, "Inverse:\n");
  cvec_print_matrix(inv_N, R, C);



}
