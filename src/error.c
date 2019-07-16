#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#include "cvec.h"

static int verbosity = 3;

void set_verbosity(int v) { verbosity = v; }

void cvec_ferr(const char *source, const char* fmt, ...)
{
  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "BG_RED"FATAL ERROR!"RESET" in "FG_BLUE"%s"RESET" %s", source, mesg);
  if (errno)
    fprintf(stderr, " (%d) %s", errno, strerror(errno));
  fprintf(stderr, "\n");

  exit(1);
}



void cvec_warn(const char *source, const char* fmt, ...)
{

  if (verbosity < 1) return;

  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "FG_YELLOW"WARNING!"RESET" in "FG_BLUE"%s"RESET": %s\n", source, mesg);
}



void cvec_info(const char* fmt, ...)
{

  if (verbosity < 2) return;

  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "FG_BLUE"%s"RESET"\n", mesg);
}



void cvec_anci(const char* fmt, ...)
{

  if (verbosity < 3) return;

  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "DIM"%s"RESET"\n", mesg);
}



void cvec_test_fail(const char *testname, const char* fmt, ...)
{
  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "BG_RED"FAILURE!"RESET" "FG_BLUE"%s"RESET": %s\n", testname, mesg);
}



void cvec_test_pass(const char *testname, const char* fmt, ...)
{
  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "BG_GREEN"PASS!"RESET" "FG_BLUE"%s"RESET": %s\n", testname, mesg);
}
