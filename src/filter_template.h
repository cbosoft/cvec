#ifndef CVECH_TYPE

#include "cvec.h"

#define CVECH_TYPE cvec_float
#define CVECH_(N) cvec_ ## N

#endif

CVECH_TYPE *CVECH_(moving_average)(CVECH_TYPE *x, cvec_uint len, cvec_uint w, CVECH_TYPE (*avfunc)(CVECH_TYPE *x, cvec_uint len));
