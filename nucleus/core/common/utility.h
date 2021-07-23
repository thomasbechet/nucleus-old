#ifndef NU_UTILITY_H
#define NU_UTILITY_H

#include <nucleus/core/common/platform.h>
#include <nucleus/core/common/result.h>

NU_API nu_result_t nu_snprintf(char *str, size_t n, const char *format, ...);
NU_API nu_result_t nu_strtof(const char *str, float *v);
NU_API nu_result_t nu_strtod(const char *str, double *v);
NU_API nu_result_t nu_strtoi(const char *str, int32_t *v);
NU_API nu_result_t nu_strtou(const char *str, uint32_t *v);
NU_API nu_result_t nu_sscanf(const char *str, const char *format, ...);

#endif