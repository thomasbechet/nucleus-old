#ifndef NU_TIMER_H
#define NU_TIMER_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>

NU_DECLARE_HANDLE(nu_timer_t);

NU_API void nu_timer_allocate(nu_timer_t *timer);
NU_API void nu_timer_free(nu_timer_t timer);
NU_API void nu_timer_start(nu_timer_t timer);
NU_API float nu_timer_get_time_elapsed(nu_timer_t timer);

#endif