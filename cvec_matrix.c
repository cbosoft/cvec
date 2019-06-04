#include <stdlib.h>
#include <stdio.h>
#include "cvec.h"

cvec_float **
cvec_new_matrix(int rows, int cols, cvec_float (*f)(int, int))
{
  cvec_float **rv = malloc(rows*sizeof(cvec_float*));
  for (int i = 0; i < rows; i++) {
    rv[i] = malloc(cols*sizeof(cvec_float*));
  }
  return rv;
}




cvec_float ** 
cvec_matmul(cvec_float **A, int rA, int cA, cvec_float **B, int rB, int cB)
{
  /*
    This is the matrix multiplication of 
    matrix A (rA x cA) by matrix B (rB x cB).

    resulting matrix is (rA x cB)

    cA must equal rB
   */
  
  if (cA != rB) {
    fprintf(stderr, 
        "Cannot multiply %dx%d matrix by %dx%d matrix.\n"
        "no. cols of first matrix must match no. rows of second.\n", 
        rA, cA, rB, cB);
    exit(1);
  }

  int R = rA, C = cB, L = cA;

  cvec_float z(int r, int c) { return 0.0; }
  cvec_float **res = cvec_new_matrix(rA, cB, &z);

  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      for (int i = 0; i < L; i++) {
        res[r][c] += A[r][i] * B[i][c];
      }
    }
  }


  

  return res;
}




cvec_float **
cvec_matrix_invert(cvec_float **A, int r, int c)
{
  cvec_float z(int r, int c) { return 0.0; }
  cvec_float **res = cvec_new_matrix(r, c, &z);
  // TODO
  return res;
}




cvec_float
cvec_matrix_determinant(cvec_float **A, int r, int c)
{
  if (r != c) {
    fprintf(stderr, "Can only find the determinant of a square matrix!\n");
    exit(1);
  }

  if (r == 2) {
    return (A[0][0] * A[1][1]) - (A[0][1]*A[1][0]);
  }
  else {
    // calc determinant for sub matrix
    // TODO
  }
}




int
cvec_matrix_is_invertible(cvec_float **A, int r, int c) 
{
  if (r != c)
    return 0;
  // TODO
}
