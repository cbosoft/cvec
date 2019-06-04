#include <stdlib.h>
#include <stdio.h>
#include <cvec.h>

int
main(int argc, const char **argv)
{
  int len = 10;
  cvec_float *sorted = malloc(sizeof(cvec_float)*len);
  cvec_float *shuffled = malloc(sizeof(cvec_float)*len);
  cvec_float *reverse = malloc(sizeof(cvec_float)*len);
  for (int i = 0; i < len; i++) {
    sorted[i] = i;
    shuffled[i] = ((cvec_float)(rand()%(len - 1)));
    reverse[i] = len - i - 1;
  }

  cvec_float *sorted_sorted = cvec_bubble_sort(sorted, len);
  cvec_float *shuffled_sorted = cvec_bubble_sort(shuffled, len);
  cvec_float *reverse_sorted = cvec_bubble_sort(reverse, len);

  for (int i = 0; i < len; i++)
    fprintf(stderr, "%.0f ", sorted_sorted[i]);
  fprintf(stderr, " %s \n", (cvec_in_order(sorted_sorted, len))?"success!":"boo!");

  for (int i = 0; i < len; i++)
    fprintf(stderr, "%.0f ", shuffled_sorted[i]);
  fprintf(stderr, " %s \n", (cvec_in_order(shuffled_sorted, len))?"success!":"boo!");

  for (int i = 0; i < len; i++)
    fprintf(stderr, "%.0f ", reverse_sorted[i]);
  fprintf(stderr, " %s \n", (cvec_in_order(reverse_sorted, len))?"success!":"boo!");

  free(sorted);
  free(shuffled);
  free(reverse);
  free(sorted_sorted);
  free(shuffled_sorted);
  free(reverse_sorted);
  return 0;
}
