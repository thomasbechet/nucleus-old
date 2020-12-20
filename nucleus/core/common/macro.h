#ifndef NU_MACRO_H
#define NU_MACRO_H

#include "platform.h"

#include <stdint.h>

/* min/max */
#define NU_MIN(a,b) (((a)<(b))?(a):(b))
#define NU_MAX(a,b) (((a)>(b))?(a):(b))
#define NU_MATCH(a,b) (strcmp(a,b)==0)

/* handle */
#define NU_DECLARE_HANDLE(name) struct name##__ { int unused; }; \
                                typedef struct name##__ *name
#ifdef NU_CXX
    #define NU_NULL_HANDLE nullptr
#else
    #define NU_NULL_HANDLE NULL
#endif
#define NU_HANDLE_SET_ID(handle, id) *((uintptr_t*)(&(handle))) = (uintptr_t)(id + 1)
#define NU_HANDLE_GET_ID(handle) ({uint32_t id; id = (uintptr_t)(handle); (id - 1);})

#endif