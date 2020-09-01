#ifndef NU_PLATFORM_H
#define NU_PLATFORM_H

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#if (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__) || defined(_WIN32))
    #define NU_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__unix__)
    #define NU_PLATFORM_UNIX
#elif defined(__APPLE__)
    #define NU_PLATFORM_APPLE
#endif

#if defined(NU_PLATFORM_WINDOWS)
    #define NU_API_EXPORT __declspec(dllexport)
    #define NU_API_IMPORT __declspec(dllimport)
    #if defined(_MSC_VER)
        #define NU_ALIGN(X) __declspec(align(X))
    #else
        #define NU_ALIGN(X) __attribute((aligned(X)))
    #endif    
#elif defined(NU_PLATFORM_UNIX)
    #define NU_API_EXPORT __attribute__((visibility("default")))
    #define NU_API_IMPORT
    #define NU_ALIGN(X) __attribute((aligned(X)))
#else
    #define NU_API_EXPORT
    #define NU_API_IMPORT
    #define NU_ALIGN(X)
    #pragma warning Unknown linkage directive import/export semantics.
#endif

#define NU_API NU_API_EXPORT

#endif