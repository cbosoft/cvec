#ifndef CVECH_TYPE

#include <cvec.h>
#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N

#endif


CVECH_TYPE CVECH_(max)(CVECH_TYPE *x, cvec_uint len);
CVECH_TYPE CVECH_(min)(CVECH_TYPE *x, cvec_uint len);
void CVECH_(get_limit)(CVECH_TYPE *in, cvec_uint len, cvec_int flags, CVECH_TYPE threshold, CVECH_TYPE *limit, cvec_uint *index);
