#ifndef CVEC_TYPE

//#error Define type and name before including template.

#define CVEC_TYPE cvec_float
#define CVEC_(N) cvec_ ## N

#include <math.h>
#include "cvec.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>

#endif

void CVEC_(hist)(CVEC_TYPE *input, cvec_uint len, CVEC_TYPE **output, CVEC_TYPE **bins, cvec_uint *nbins)
{

  if ((*nbins) <= 0)
    (*nbins) = (cvec_uint)pow((CVEC_TYPE)len, 0.5);

  CVEC_TYPE binw, min = CVEC_FLOAT_MAX, max = CVEC_FLOAT_MIN;
  for (cvec_uint i = 0; i < len; i++) {
    if (input[i] < min)
      min = input[i];
    if (input[i] > max)
      max = input[i];
  }
  
  // are they equalish?
  if ((max - min) < 0.00001525878906) {
    min = max - 2.0;
    max = max + 2.0;
  }

  binw = (max - min) / ((CVEC_TYPE) (*nbins));

  (*output) = CVEC_(zeros)((*nbins));

  for (cvec_uint i = 0; i < len; i++) {
    cvec_uint bidx = (cvec_uint)(input[i] / binw);
    if (bidx > (*nbins) || bidx < 0)
      CVEC_(ferr)("cvec_hist", "bin width calc failed.");
    (*output)[bidx] += 1.0;
  }

  (*bins) = malloc(sizeof(CVEC_TYPE)*len);
  for (cvec_uint i = 0; i < (*nbins); i++) {
    (*bins)[i] = ((CVEC_TYPE)i + 0.5)*binw;
  }

}

typedef struct autocorr_data_t {
  CVEC_TYPE *x;
  CVEC_TYPE *y;
  cvec_uint len;

  cvec_uint from;
  cvec_uint to;

  CVEC_TYPE binw;
  CVEC_TYPE *res_y;
  cvec_uint nbins;
  CVEC_TYPE maxlag;

  cvec_uint thread_id;
  cvec_uint progress;
} autocorr_data_t;

void *corr(void *ptr)
{
  autocorr_data_t *adt = (autocorr_data_t *)ptr;

  adt->progress = 0;

  for (cvec_uint i = adt->from; i < adt->to; i++) {
    for (cvec_uint j = i+1; j < adt->len; j++) {

      CVEC_TYPE dt = adt->x[j] - adt->x[i];

      if (dt > adt->maxlag)
        break;

      cvec_uint ibin = (cvec_uint)(dt/adt->binw);

      if (ibin >= adt->nbins)
        break;

      adt->res_y[ibin] += adt->y[i]*adt->y[j];

    }
    adt->progress += 1;
  }

  return NULL;
}


void CVEC_(autocorr)(
    CVEC_TYPE* x, 
    CVEC_TYPE *y, 
    cvec_uint len, 
    CVEC_TYPE **res_x, 
    CVEC_TYPE **res_y, 
    cvec_uint *nbins,
    CVEC_TYPE *bin_width)
{
  // time correlation
  //
  // for each data point, for each lag dt, get product, sum:
  //    tcorr = VxV(l) = int _-inf^inf V(t)xV(t+l) dt
  // normalise wrt zero lag time

  // for discrete signals, autocorr R is given by:
  // R = sum_0^N y(n) av(y(n-l))

  if (x == NULL || y == NULL)
    CVEC_(ferr)(
        "cvec_autocorr", 
        "autocorrelation input cannot be NULL");

  if (!CVEC_(in_order)(x, len))
    CVEC_(ferr)(
        "cvec_autocorr", 
        "autocorrelation input must be in time-order (i.e. sorted by x)");

  if (nbins == NULL)
    CVEC_(ferr)(
        "cvec_autocorr", 
        "nbins must point to a valid memory location (i.e. cannot be NULL)");

  if (len > CVEC_CORR_LONG_LEN)
    CVEC_(warn)(
        "cvec_autocorr", 
        "lengths over %u may take some time (length = %d)", 
        CVEC_CORR_LONG_LEN, len);

  CVEC_TYPE *dx = CVEC_(diff)(x, len);
  CVEC_TYPE mindt = CVEC_FLOAT_MAX, maxdt = -CVEC_FLOAT_MAX;

  for (cvec_uint i = 0; i < (len-1); i++) {
    if (dx[i] < mindt)
      mindt = dx[i];
    if (dx[i] > maxdt)
      maxdt = dx[i];
  }
  free(dx);

  CVEC_TYPE binw;
  if ((*bin_width) <= 0.0) {
    binw = mindt * 1.0;
    if (bin_width != NULL) (*bin_width) = binw;
    fprintf(stderr, "  Bin width not given; using smallest: %f\n", binw);
  }
  else {
    binw = (*bin_width);
  }

  (*nbins) = ceil((x[len-1] - x[0]) / binw);

  (*res_x) = malloc(sizeof(CVEC_TYPE)*(*nbins));
  CVEC_TYPE *raw_res_y = CVEC_(zeros)((*nbins));
  
  pthread_t threads[CVEC_NJOBS];
  autocorr_data_t args[CVEC_NJOBS];
  cvec_uint seg = len/CVEC_NJOBS;

  for (cvec_uint i = 0; i < CVEC_NJOBS; i++) {
    
    args[i].x = x;
    args[i].y = y;
    args[i].len = len;
    args[i].nbins = (*nbins);
    args[i].res_y = raw_res_y;
    args[i].from = i*seg;
    args[i].binw = binw;
    args[i].maxlag = 60.0;
    args[i].to = (i == CVEC_NJOBS-1)?(len):(i+1)*seg;
    args[i].thread_id = i;
    args[i].progress = 0;
    
    pthread_create(&threads[i], NULL, corr, &args[i]);

  }
  
  if (len > CVEC_CORR_LONG_LEN) {

    cvec_float perc = 0;

    do {

      cvec_uint total_progress = 0;

      for (cvec_uint i = 0; i < CVEC_NJOBS; i++)
        total_progress += args[i].progress;

      perc = 100.0 * ((double)total_progress) / ((double)len);

      progressbar("  Correlation progress: ", perc);

      sleep(1);

    } while (perc < 100);

    fprintf(stderr, "\n");

  }

  for (cvec_uint i = 0; i < CVEC_NJOBS; i++) {
    pthread_join(threads[i], NULL);
  }

  for (cvec_uint i = 0; i < (*nbins); i++)
    (*res_x)[i] = ((CVEC_TYPE)(i + 1)) * binw;

  // get max and normalize output
  CVEC_TYPE maxv = CVEC_(max)(raw_res_y, (*nbins));
  CVEC_TYPE invmaxv = 1.0 / maxv;
  CVEC_TYPE norm(CVEC_TYPE v) { return v * invmaxv; }
  (*res_y) = CVEC_(apply)(raw_res_y, (*nbins), &norm);

  free(raw_res_y);
}



CVEC_TYPE CVEC_(std)(CVEC_TYPE *x, cvec_uint len)
{
  CVEC_TYPE average = CVEC_(average)(x, len);
  CVEC_TYPE getsquaredev(CVEC_TYPE v) { return pow(average-v,2.0); }
  CVEC_TYPE *square_deviation = CVEC_(apply)(x, len, &getsquaredev);
  CVEC_TYPE sum_square_deviation = CVEC_(sum)(square_deviation, len);
  CVEC_TYPE standard_deviation = sum_square_deviation / ((CVEC_TYPE)len);
  standard_deviation = pow(standard_deviation, 0.5);
  free(square_deviation);
  return standard_deviation;
}







