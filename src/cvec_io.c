#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cvec.h"

FILE *cvec_safe_fopen(const char *path, const char *t)
{
  FILE *rv = fopen(path, t);
  if (rv == NULL) {
    fprintf(stderr, "\033[31mFATAL ERROR!\033[0m cvec_safe_fopen: (%d) %s\n", errno, strerror(errno));
    exit(1);
  }
  return rv;
}





void cvec_write_csv(cvec_float **data, cvec_uint ncols, cvec_uint *nlines, const char *csv_path)
{
  FILE *csvf = cvec_safe_fopen(csv_path, "w");

  cvec_uint maxlen = 0;
  for (cvec_uint i = 0; i < ncols; i++) {
    if (nlines[i] > maxlen)
      maxlen = nlines[i];
  }

  for (cvec_uint l = 0; l < maxlen; l++) {

    for (cvec_uint c = 0; c < ncols; c++) {

      if (c > 0)
        fprintf(csvf, ",");

      if (l < nlines[c])
        fprintf(csvf, "%f", data[c][l]);

    }

    fprintf(csvf, "\n");

  }

  fclose(csvf);
}




void cvec_write_csv_xy(cvec_float *x, cvec_float *y, cvec_uint len, const char *csv_path)
{
  FILE *csvf = cvec_safe_fopen(csv_path, "w");

  for (cvec_uint l = 0; l < len; l++) {
    fprintf(csvf, "%f,%f\n", x[l], y[l]);
  }

  fclose(csvf);
}
