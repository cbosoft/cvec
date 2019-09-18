#ifndef CVECH_TYPE
#error Define type and name before including template.
#endif


void CVECH_(hist)(CVECH_TYPE *input, cvec_uint len, CVECH_TYPE **output, CVECH_TYPE **bins, cvec_uint *nbins);
void CVECH_(autocorr)( CVECH_TYPE* x, CVECH_TYPE *y, cvec_uint len, CVECH_TYPE **res_x, CVECH_TYPE **res_y, cvec_uint *nbins, CVECH_TYPE *bin_width, CVECH_TYPE maxlag);
CVECH_TYPE CVECH_(std)(CVECH_TYPE *x, cvec_uint len);
