#include <nucleus/core/utils/timer.h>

#if defined(NU_PLATFORM_WINDOWS)
#include <Windows.h>
void nu_timer_start(nu_timer_t *timer)
{
    LARGE_INTEGER *p = (LARGE_INTEGER*)timer;
    QueryPerformanceCounter(p);
}
float nu_timer_get_time_elapsed(nu_timer_t *timer)
{
    LARGE_INTEGER frequency, t1;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);

    LARGE_INTEGER *t0 = (LARGE_INTEGER*)timer;

    double delta = (t1.QuadPart - t0->QuadPart) * 1000.0 / frequency.QuadPart;
    return (float)delta;
}
#elif defined(NU_PLATFORM_UNIX)
#include <time.h>
void nu_timer_start(nu_timer_t *timer)
{
    *((clock_t*)timer) = clock();
}
float nu_timer_get_time_elapsed(nu_timer_t *timer)
{
    return (float)(clock() - *((clock_t*)timer)) * 1000.0 / CLOCKS_PER_SEC;
}
#endif