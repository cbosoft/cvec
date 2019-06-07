#include "cvec.h"

bool
cvec_in_order(cvec_float* in, cvec_uint len) {
  for (cvec_uint i = 0, j = 1; j < len; i++, j++) {
    if (in[j] < in[i])
      return 0;
  }
  return 1;
}

cvec_float *
cvec_bubble_sort(cvec_float *unsorted, cvec_uint len) {
  cvec_float *rv = cvec_copy(unsorted, len);
  
  cvec_uint changed = 0;
  do {
    changed = 0;
    for (cvec_uint i = 0, j = 1; j < len; i++, j++) {
      if (rv[j] < rv[i]) {
        cvec_float temp = rv[j];
        rv[j] = rv[i];
        rv[i] = temp;
        changed = 1;
      }
    }
  } while (changed);
  return rv;
}




cvec_float *
cvec_sort(cvec_float *unsorted, cvec_uint len) {
  // for each sort method, choose best one
  // TODO
  cvec_float *rv = cvec_bubble_sort(unsorted, len);
  return rv;
}
