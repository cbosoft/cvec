#include <stdlib.h>

#include "cvec.h"

#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N
#include "sort_template.c"
