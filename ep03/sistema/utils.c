#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utils.h"

string_t markerName(string_t baseName, int n)
{
  string_t mark = (string_t) malloc( (strlen(baseName)+1) + (log10(n)+1) + 1 );

  sprintf(mark, "%s_%u", baseName,n);

  return mark;
}

int isPot2(int n)
{
  int k;
  return (k = log2(n)) == log2(n) ;
}

double timestamp(void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}