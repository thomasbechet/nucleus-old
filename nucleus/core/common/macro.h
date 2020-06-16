#ifndef NU_MACRO_H
#define NU_MACRO_H

#include <string.h>

#define NU_MIN(a,b) (((a)<(b))?(a):(b))
#define NU_MAX(a,b) (((a)>(b))?(a):(b))
#define NU_MATCH(a,b) (strcmp(a,b)==0)

#endif