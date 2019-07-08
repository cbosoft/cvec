#include <stdlib.h>

#include "cvec.h"




bool cvec_in_order(cvec_float* in, cvec_uint len)
{
  for (cvec_uint i = 0, j = 1; j < len; i++, j++) {
    if (in[j] < in[i])
      return 0;
  }
  return 1;
}




void cvec_bubble_sort(cvec_float *unsorted, cvec_uint len, cvec_uint **sorted_indices, cvec_float **sorted_values)
{
  cvec_float *svs = cvec_copy(unsorted, len);
  cvec_uint *sids = malloc(len*sizeof(cvec_uint));
  for (cvec_uint i = 0; i < len; i++) sids[i] = i;

  cvec_uint changed = 0;
  do {
    changed = 0;
    for (cvec_uint i = 0, j = 1; j < len; i++, j++) {
      if (svs[j] < svs[i]) {

        cvec_float temp = svs[j];
        svs[j] = svs[i];
        svs[i] = temp;

        cvec_int temp2 = sids[j];
        sids[j] = sids[i];
        sids[i] = temp2;

        changed = 1;
      }
    }
  } while (changed);

  (*sorted_indices) = sids;
  (*sorted_values) = svs;

}




void cvec_sort(cvec_float *unsorted, cvec_uint len, cvec_uint **sorted_indices, cvec_float **sorted_values)
{
  // for each sort method, choose best one
  // TODO
  cvec_bubble_sort(unsorted, len, sorted_indices, sorted_values);
}
