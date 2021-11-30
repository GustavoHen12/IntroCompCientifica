#ifndef __UTILS__
#define __UTILS__

#include <stdlib.h>
#include <sys/time.h>

typedef char * string_t;

string_t markerName(string_t baseName, int n);
int isPot2(int n);
double timestamp(void);

#endif
