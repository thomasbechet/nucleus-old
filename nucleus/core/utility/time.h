#ifndef NU_TIME_H
#define NU_TIME_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/api/allocator.h>

#define NU_TIME_FORMAT_LENGTH 80
typedef uint64_t nu_time_t;

NU_API nu_time_t nu_time_now(void);
NU_API void nu_time_format(nu_time_t time, char buf[NU_TIME_FORMAT_LENGTH]);

NU_DECLARE_HANDLE(nu_timer_t);

NU_API nu_timer_t nu_timer_allocate(nu_allocator_t allocator);
NU_API void nu_timer_free(nu_timer_t timer);
NU_API void nu_timer_start(nu_timer_t timer);
NU_API float nu_timer_get_time_elapsed(nu_timer_t timer);

#endif