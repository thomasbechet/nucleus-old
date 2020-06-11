#ifndef NU_PLATFORM_H
#define NU_PLATFORM_H

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
#elif defined(NU_PLATFORM_UNIX)
    #define NU_API_EXPORT __attribute__((visibility("default")))
    #define NU_API_IMPORT
#else
    #define NU_API_EXPORT
    #define NU_API_IMPORT
    #pragma warning Unknown linkage directive import/export semantics.
#endif

#define NU_API NU_API_EXPORT

#endif