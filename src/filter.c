#include <stdlib.h>
#include <stdio.h>
#include "cvec.h"

#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N
#include "filter_template.c"
