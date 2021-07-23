#include <nucleus/core/common/utility.h>

nu_result_t nu_snprintf(char *str, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int32_t code = vsnprintf(str, n, format, args);
    va_end(args);
    return code >= 0 ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nu_strtof(const char *str, float *v)
{
    char *t;
    *v = strtof(str, &t);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}
nu_result_t nu_strtod(const char *str, double *v)
{
    char *t;
    *v = strtod(str, &t);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}
nu_result_t nu_strtoi(const char *str, int32_t *v)
{
    char *t;
    *v = strtol(str, &t, 10);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}
nu_result_t nu_strtou(const char *str, uint32_t *v)
{
    char *t;
    *v = strtoul(str, &t, 10);
    return (*t != '\0') ? NU_FAILURE : NU_SUCCESS;
}