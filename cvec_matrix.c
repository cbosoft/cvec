#include <stdlib.h>
#include <stdio.h>
#include "cvec.h"

cvec_float
cvec_matgen_zero(cvec_uint r, cvec_uint c) {
  return 0.0;
}





cvec_float **
cvec_matrix_new(cvec_uint R, cvec_uint C, cvec_float (*f)(cvec_uint, cvec_uint))
{
  cvec_float **rv = malloc(R*sizeof(cvec_float*));
  for (cvec_uint r = 0; r < R; r++) {
    rv[r] = malloc(C*sizeof(cvec_float));
    for (cvec_uint c = 0; c < C; c++) {
      rv[r][c] = f(r, c);
    }
  }
  return rv;
}




cvec_float **
cvec_matrix_copy(cvec_float **A, cvec_uint R, cvec_uint C)
{
  cvec_float **rv = malloc(R*sizeof(cvec_float*));
  for (cvec_uint r = 0; r < R; r++) {
    rv[r] = malloc(C*sizeof(cvec_float));
    for (cvec_uint c = 0; c < C; c++) {
      rv[r][c] = A[r][c];
    }
  }
  return rv;
}



cvec_float **
cvec_matrix_apply(cvec_float **A, cvec_uint R, cvec_uint C, cvec_float (*f)(cvec_float))
{
  cvec_float **rv = cvec_matrix_copy(A, R, C);
  for (cvec_uint r = 0; r <R; r++) {
    for (cvec_uint c = 0; c < C; c++) {
      rv[r][c] = f(rv[r][c]);
    }
  }
  return rv;
}




cvec_float ** 
cvec_matrix_cross(cvec_float **A, cvec_uint rA, cvec_uint cA, cvec_float **B, cvec_uint rB, cvec_uint cB)
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

  cvec_uint R = rA, C = cB, L = cA;

  cvec_float z(cvec_uint r, cvec_uint c) { return 0.0; }
  cvec_float **res = cvec_matrix_new(rA, cB, &z);

  for (cvec_uint r = 0; r < R; r++) {
    for (cvec_uint c = 0; c < C; c++) {
      for (cvec_uint i = 0; i < L; i++) {
        res[r][c] += A[r][i] * B[i][c];
      }
    }
  }
  return res;
}




cvec_float **
cvec_matrix_invert(cvec_float **A, cvec_uint R, cvec_uint C)
{
  if (R != C) {
    fprintf(stderr, "Warning! Inversion failed: can only invert square matrix.\n");
    return NULL;
  }

  cvec_float det = cvec_matrix_determinant(A, R, C);

  if (det == 0.0) {
    fprintf(stderr, "Warning! Inversion failed: matrix is non-invertible.\n");
    return NULL;
  }
  
  cvec_float **moc = cvec_matrix_of_cofactors(A, R, C);
  cvec_float **adj = cvec_matrix_transpose(moc, R, C);
  free(moc);
  
  cvec_float divdet(cvec_float v) { return v / det; }
  cvec_float **inverse = cvec_matrix_apply(adj, R, C, &divdet);
  free(adj);
  return inverse;
}



cvec_float **
cvec_matrix_minor(cvec_float **A, cvec_uint R, cvec_uint C, cvec_uint notR, cvec_uint notC)
{
  cvec_float **rv = cvec_matrix_new(R-1, C-1, &cvec_matgen_zero);
  
  cvec_uint r = 0, c = 0;
  for (cvec_uint or = 0; or < R; or++) {
    if (or == notR)
      continue;

    c = 0;
    for (cvec_uint oc = 0; oc < C; oc++) {
      if (oc == notC)
        continue;

      rv[r][c] = A[or][oc];

      c++;
    }

    r++;
  }
  return rv;
}




cvec_float **
cvec_matrix_of_minors(cvec_float **A, cvec_uint R, cvec_uint C) 
{
  cvec_float **rv = cvec_matrix_new(R, C, &cvec_matgen_zero);
  for (cvec_uint r = 0; r < R; r++) {
    for (cvec_uint c = 0; c < C; c++) {
      cvec_float **minor = cvec_matrix_minor(A, R, C, r, c);
      rv[r][c] = cvec_matrix_determinant(minor, R-1, C-1);
      free(minor);
    }
  }
  return rv;
}



cvec_float **
cvec_matrix_of_cofactors(cvec_float **A, cvec_uint R, cvec_uint C) {
  cvec_float **rv = cvec_matrix_new(R, C, &cvec_matgen_zero);
  cvec_float **matrix_of_minors = cvec_matrix_of_minors(A, R, C);
  cvec_float sign = 1.0;
  for (cvec_uint r = 0; r < R; r++) {
    for (cvec_uint c = 0; c < C; c++) {
      rv[r][c] = matrix_of_minors[r][c] * sign;
      sign *= -1.0;
    }
  }
  free(matrix_of_minors);
  return rv;
}




cvec_float **
cvec_matrix_transpose(cvec_float **A, cvec_uint R, cvec_uint C) {
  cvec_float **rv = cvec_matrix_new(C, R, &cvec_matgen_zero);
  for (cvec_uint r = 0; r < R; r++) {
    for (cvec_uint c = 0; c < C; c++) {
      rv[r][c] = A[c][r];
    }
  }
  return rv;
}




cvec_float
cvec_matrix_determinant(cvec_float **A, cvec_uint R, cvec_uint C)
{
  if (R != C) {
    fprintf(stderr, "Can only find the determinant of a square matrix!\n");
    exit(1);
  }

  if (R == 2) {
    return (A[0][0] * A[1][1]) - (A[0][1]*A[1][0]);
  }
  // calc determinant for sub matrix
  cvec_float det = 0.0;
  cvec_float sign = 1.0;
  for (cvec_uint c = 0; c < C; c++) {
    cvec_float **minor = cvec_matrix_minor(A, R, C, 0, c);
    det += sign * A[0][c] * cvec_matrix_determinant(minor, R-1, C-1);
    sign = sign * -1.0;
    free(minor);
  }
  return det;
}




bool
cvec_matrix_is_invertible(cvec_float **A, cvec_uint R, cvec_uint C) 
{
  if (R != C)
    return 0;
  if (cvec_matrix_determinant(A, R, C) != 0)
    return 1;
  return 0;
}




void
cvec_print_matrix(cvec_float **A, cvec_uint R, cvec_uint C)
{
  for (cvec_uint r = 0; r < R; r++) {
    fprintf(stderr, "│ ");
    for (cvec_uint c = 0; c < C; c++) {
      fprintf(stderr, "%.1e ", A[r][c]);
    }
    fprintf(stderr, "│\n");
  }
}




void
cvec_matrix_free(cvec_float **A, cvec_uint R, cvec_uint C) 
{
  for (cvec_uint r = 0; r < R; r++) {
    free(A[r]);
  }
  free(A);
}
