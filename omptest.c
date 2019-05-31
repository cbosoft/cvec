#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <cvec.h>

int
time_raw(double from, double to, int len) {
  struct timeval stop, start;

  gettimeofday(&start, NULL);
  double *rv = malloc(len*sizeof(double));
  double step = (to - from) / ((double)(len - 1));
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((double)i)) + from;
  }
  gettimeofday(&stop, NULL);

  free(rv);

  return stop.tv_usec - start.tv_usec;
}

int
time_parallel(double from, double to, int len) {
  struct timeval stop, start;

  gettimeofday(&start, NULL);
  double *rv = cvec_linspace(from, to, len);
  gettimeofday(&stop, NULL);
  free(rv);

  if ((stop.tv_sec - start.tv_sec) > 0) {
    fprintf(stderr, "took longer than a second");
    exit(1);
  }

  return stop.tv_usec - start.tv_usec;
}







int
main(int argc, const char **argv) {
  int len = 10000, repeats = 100;
  double from = 0.0, to = 1.0;
  int *parallel_times = malloc(repeats*sizeof(int)), *raw_times = malloc(repeats*sizeof(int));

  for (int i = 0 ; i < repeats; i++) {
    parallel_times[i] = time_parallel(from, to, len);
    raw_times[i] = time_raw(from, to, len);
  }

  int average_parallel = cvec_int_average(parallel_times, len);

}
