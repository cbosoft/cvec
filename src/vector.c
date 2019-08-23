#include "cvec.h"

#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N
#define FLOATING_TYPE
#include "vector_template.c"
