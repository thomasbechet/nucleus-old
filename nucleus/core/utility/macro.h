#ifndef NU_MACRO_H
#define NU_MACRO_H

#include <stdint.h>
#include <assert.h>

/* min/max */
#define NU_MIN(a,b) (((a)<(b))?(a):(b))
#define NU_MAX(a,b) (((a)>(b))?(a):(b))
#define NU_MATCH(a,b) (strcmp(a,b)==0)

/* handle */
#define NU_DECLARE_HANDLE(name) struct name##__ { int unused; }; \
                                typedef struct name##__ *name
#ifdef NU_CXX
    #define NU_NULL_HANDLE nullptr
    #define NU_HANDLE_SET_ID(handle, id) handle = static_cast<decltype(handle)>((void*)(uintptr_t)(id + 1))
    #define NU_HANDLE_GET_ID(handle, id) id = ((uintptr_t)(handle) - 1)
#else
    #define NU_NULL_HANDLE NULL
    #define NU_HANDLE_SET_ID(handle, id) handle = (void*)(uintptr_t)(id + 1)
    #define NU_HANDLE_GET_ID(handle, id) id = ((uintptr_t)(handle) - 1)
#endif

/* assert */
#ifdef NU_DEBUG
    #define NU_ASSERT(x) assert(x)
#else
    #define NU_ASSERT(x) x
#endif

/* noop */
#define NU_NOOP

/* identity */
#define NU_IDENTITY(x) x

#endif