#ifndef NU_TIMER_H
#define NU_TIMER_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_timer_t);

NU_API void nu_timer_start(nu_timer_t *timer);
NU_API float nu_timer_get_time_elapsed(nu_timer_t *timer);

#endif