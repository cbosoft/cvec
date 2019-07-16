#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "cvec.h"

void progressbar(const char* message, cvec_float progress)
{
      struct winsize ws;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

      cvec_uint width_total = (cvec_uint)ws.ws_col;
      char *percstr = calloc(10, sizeof(char));
      sprintf(percstr, "%.0f%%", progress);
      cvec_uint barl = width_total - strlen(message) - 2 - strlen(percstr) - 2;
      cvec_uint pind = ((cvec_uint)(progress*barl)) / 100;
      
      fprintf(stdout, "\r%s ▕", message);
      for (cvec_uint i = 0; i < pind; i++) fprintf(stdout, "▉");
      for (cvec_uint i = pind; i < barl; i++) fprintf(stdout, " ");

      fprintf(stdout, "▏ %s", percstr);
      fflush(stdout);

}




void progressbar_finish(void)
{
  fprintf(stdout, "\n");
}
