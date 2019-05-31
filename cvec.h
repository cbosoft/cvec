#pragma once

double *cvec_linspace(double from, double to, int len);
double *cvec_logspace(double from, double to, int len);
double *cvec_apply(double* in, int len, double (*f)());
double cvec_average(double* in, int len);
double cvec_sum(double* in, int len);


int *cvec_int_linspace(int from, int to, int len);
int *cvec_int_logspace(int from, int to, int len);
int *cvec_int_apply(int* in, int len, int (*f)());
int cvec_int_average(int* in, int len);
int cvec_int_sum(int* in, int len);
