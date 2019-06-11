#include <stdio.h>
#include "cvec.h"

void
cvec_write_csv(cvec_float **data, cvec_uint ncols, cvec_uint nlines, const char *csv_path)
{
  FILE *csvf = fopen(csv_path, "w");

  for (cvec_uint l = 0; l < nlines; l++) {
    for (cvec_uint c = 0; c < ncols; c++) {
      fprintf(csvf, "%f,", data[c][l]);
    }
    fprintf(csvf, "\n");
  }

  fclose(csvf);
}
