#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <limits.h>
#include <cvec.h>
#include <math.h>


cvec_float
square(cvec_float in) {
  return pow(in, 2.0);
}

cvec_float *
raw_linspace(cvec_float from, cvec_float to, int len) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));
  cvec_float step = (to - from) / ((cvec_float)(len - 1));
  for (int i = 0; i < len; i++) {
    rv[i] = (step*((cvec_float)i)) + from;
  }
  return rv;
}


cvec_float *
raw_apply(cvec_float* in, int len, cvec_float (*f)(cvec_float)) {
  cvec_float *rv = malloc(len*sizeof(cvec_float));
  for (int i = 0; i < len; i++){
    rv[i] = f(in[i]);
  }
  return rv;
}

cvec_int
time_raw(cvec_float from, cvec_float to, int len) {
  struct timeval stop, start;

  gettimeofday(&start, NULL);
  cvec_float *rv1 = raw_linspace(from, to, len);
  cvec_float *rv2 = raw_apply(rv1, len, &square);
  gettimeofday(&stop, NULL);
  free(rv1);
  free(rv2);
  
  cvec_int dusec = (cvec_int)(stop.tv_usec - start.tv_usec);
  cvec_int dsec = (cvec_int)(stop.tv_sec - start.tv_sec);
  dusec = (1000000000 * dsec) + dusec;
  return dusec;
}

cvec_int
time_parallel(cvec_float from, cvec_float to, int len) {
  struct timeval stop, start;

  gettimeofday(&start, NULL);
  cvec_float *rv1 = cvec_linspace(from, to, len);
  cvec_float *rv2 = cvec_apply(rv1, len, &square);
  gettimeofday(&stop, NULL);
  free(rv1);
  free(rv2);

  cvec_int dusec = (cvec_int)(stop.tv_usec - start.tv_usec);
  cvec_int dsec = (cvec_int)(stop.tv_sec - start.tv_sec);
  dusec = (1000000000 * dsec) + dusec;
  return dusec;
}







int
main(int argc, const char **argv) {
  

  int len = 1000*1000, repeats = 100;

  fprintf(stderr, 
      "Creating array of %d numbers between 0 and 1, \n"
      "linearly spaced, and squaring them. This will \n"
      "be repeated %d times, the time taken recorded.\n", len, repeats);

  double from = 0.0, to = 1.0;
  cvec_int *parallel_times = malloc(repeats*sizeof(cvec_int)), *raw_times = malloc(repeats*sizeof(cvec_int));

  fprintf(stderr, "\nNon-parallel test");
  for (int i = 0 ; i < repeats; i++) {
    raw_times[i] = time_raw(from, to, len);
    fprintf(stderr, ".");
  }

  fprintf(stderr, "\nParallel test");
  for (int i = 0 ; i < repeats; i++) {
    parallel_times[i] = time_parallel(from, to, len);
    fprintf(stderr, ".");
  }

  cvec_int average_parallel = cvec_int_average(parallel_times, repeats);
  cvec_int average_raw = cvec_int_average(raw_times, repeats);

  cvec_int dt = average_raw - average_parallel;

  cvec_float dt_sec = ((cvec_float)dt) / 1000000000.0;

  fprintf(stderr, "\n\nResults\n---\nraw: %lld us\nparallel: %lld us\nTime saved using parallel: %lld us (%f s)\n", average_raw, average_parallel, dt, dt_sec);


}
