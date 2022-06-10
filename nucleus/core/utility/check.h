#ifndef NU_CHECK_H
#define NU_CHECK_H

#define _NU_S(x) #x
#define _NU_S_(x) _NU_S(x)
#define _NU_S__LINE__ _NU_S_(__LINE__)

#ifdef NU_DEBUG
    #define _NU_CHECK(check, action, file, line, logger, message, ...) \
        if (!(check)) { \
            nu_error(logger, "Invalid check ("file":"line"): "message, ##__VA_ARGS__); \
            action; \
        }
#else
    #define _NU_CHECK(check, action, file, line, logger, message, ...) \
        if (!(check)) { \
            nu_error(logger, "Invalid check: "message, ##__VA_ARGS__); \
            action; \
        }
#endif

#define NU_CHECK(check, action, logger, message, ...) \
    _NU_CHECK(check, action, __FILE__, _NU_S__LINE__, logger, message, ##__VA_ARGS__)

#endif