#ifndef CVECH_TYPE
#define CVECH_TYPE double
#define CVECH_(N) cvec_test_ ## N
#define cvec_uint unsigned int
#endif


// creating new vectors {{{

CVECH_TYPE *CVECH_(linspace)(CVECH_TYPE from, CVECH_TYPE to, cvec_uint len);
CVECH_TYPE *CVECH_(logspace)(CVECH_TYPE from, CVECH_TYPE to, cvec_uint len);
CVECH_TYPE *CVECH_(zeros)(cvec_uint len);
CVECH_TYPE *CVECH_(ones)(cvec_uint len);
CVECH_TYPE *CVECH_(copy)(CVECH_TYPE *source, cvec_uint len);

// simple vector manipulation

CVECH_TYPE CVECH_(add)(CVECH_TYPE v1, CVECH_TYPE v2);
CVECH_TYPE CVECH_(subtract)(CVECH_TYPE v1, CVECH_TYPE v2);
CVECH_TYPE CVECH_(multiply)(CVECH_TYPE v1, CVECH_TYPE v2);
CVECH_TYPE CVECH_(divide)(CVECH_TYPE v1, CVECH_TYPE v2);
CVECH_TYPE CVECH_(pow)(CVECH_TYPE v1, CVECH_TYPE v2);
CVECH_TYPE *CVECH_(apply)(CVECH_TYPE* in, cvec_uint len, CVECH_TYPE (*f)(CVECH_TYPE));
CVECH_TYPE *CVECH_(apply2)(CVECH_TYPE* in1, CVECH_TYPE *in2, cvec_uint len, CVECH_TYPE (*f)(CVECH_TYPE, CVECH_TYPE));
CVECH_TYPE *CVECH_(slice)(CVECH_TYPE *source, cvec_uint len, cvec_uint start, cvec_uint stop, cvec_uint skip);
CVECH_TYPE *CVECH_(cat)(CVECH_TYPE *source, cvec_uint len, CVECH_TYPE *add, cvec_uint addlen);
CVECH_TYPE *CVECH_(diff)(CVECH_TYPE *x, cvec_uint len);
CVECH_TYPE *CVECH_(rearrange)(CVECH_TYPE *x, cvec_uint len, cvec_uint *arrangement, cvec_uint alen);
void CVECH_(set_constant)(CVECH_TYPE *x, cvec_uint len, CVECH_TYPE v);

// properties of vector

CVECH_TYPE CVECH_(get_fit_sumse)(CVECH_TYPE *x, CVECH_TYPE *y, cvec_uint len, CVECH_TYPE *coefs, cvec_uint ncoefs);
CVECH_TYPE *CVECH_(polyfit)(CVECH_TYPE *x, CVECH_TYPE *y, cvec_uint len, cvec_uint degree);
CVECH_TYPE *CVECH_(linearfit)(CVECH_TYPE *x, CVECH_TYPE *y, cvec_uint len);
CVECH_TYPE CVECH_(interpolate)(CVECH_TYPE *x, CVECH_TYPE *y, cvec_uint len, CVECH_TYPE ix);

// stats
CVECH_TYPE CVECH_(average)(CVECH_TYPE * in, cvec_uint len);
CVECH_TYPE CVECH_(mean)(CVECH_TYPE * in, cvec_uint len);
CVECH_TYPE CVECH_(median)(CVECH_TYPE * in, cvec_uint len);
CVECH_TYPE CVECH_(sum)(CVECH_TYPE * in, cvec_uint len);
CVECH_TYPE CVECH_(prod)(CVECH_TYPE * in, cvec_uint len);
