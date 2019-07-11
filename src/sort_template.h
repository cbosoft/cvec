#ifndef CVECH_TYPE
#error Define type and name before including template.
#endif

bool CVECH_(in_order)(CVECH_TYPE* in, cvec_uint len);
void CVECH_(bubble_sort)(CVECH_TYPE *unsorted, cvec_uint len, cvec_uint **sorted_indices, CVECH_TYPE **sorted_values);
void CVECH_(sort)(CVECH_TYPE *unsorted, cvec_uint len, cvec_uint **sorted_indices, CVECH_TYPE **sorted_values);
