#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#include "cvec.h"

void
cvec_ferr(const char *source, const char* fmt, ...)
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



void
cvec_warn(const char *source, const char* fmt, ...)
{
  size_t mesglen = 256;
  char *mesg = calloc(mesglen, sizeof(char));

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, mesglen, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "FG_YELLOW"WARNING!"RESET" in "FG_BLUE"%s"RESET" %s", source, mesg);
  if (errno)
    fprintf(stderr, " (%d) %s", errno, strerror(errno));
  fprintf(stderr, "\n");
}
