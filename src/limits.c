#include <stdlib.h>

#include "cvec.h"

#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N
#include "limits_template.c"
