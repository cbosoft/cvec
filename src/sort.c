#include <stdlib.h>

#include "cvec.h"

// TODO template this file


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
  cvec_uint ret_ids = 0, ret_vals = 0;

  if (sorted_indices != NULL) {
    (*sorted_indices) = cvec_uint_linspace(0, len, len);
    ret_ids = 1;
  }

  if (sorted_values != NULL) {
    (*sorted_values) = cvec_copy(unsorted, len);
    ret_vals = 1;
  }

  if (ret_ids == 0 && ret_vals == 0)
    return;

  cvec_uint changed = 0;
  do {
    changed = 0;
    for (cvec_uint i = 0, j = 1; j < len; i++, j++) {
      if ((*sorted_values)[j] < (*sorted_values)[i]) {
        
        if (ret_vals) {
          cvec_float temp = (*sorted_values)[j];
          (*sorted_values)[j] = (*sorted_values)[i];
          (*sorted_values)[i] = temp;
        }
        
        if (ret_ids) {
          cvec_int temp = (*sorted_indices)[j];
          (*sorted_indices)[j] = (*sorted_indices)[i];
          (*sorted_indices)[i] = temp;
        }

        changed = 1;
      }
    }
  } while (changed);

}




void cvec_sort(cvec_float *unsorted, cvec_uint len, cvec_uint **sorted_indices, cvec_float **sorted_values)
{
  // for each sort method, choose best one
  // TODO
  cvec_bubble_sort(unsorted, len, sorted_indices, sorted_values);
}
