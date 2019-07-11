#include <stdlib.h>

#include "cvec.h"

#define CVEC_TYPE cvec_int
#define CVEC_(N) cvec_int_ ## N
#include "sort_template.c"
