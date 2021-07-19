#include <nucleus/core/utils/timer.h>

#include <nucleus/core/coresystem/memory/memory.h>

#if defined(NU_PLATFORM_WINDOWS)
#include <Windows.h>

typedef struct {
    LARGE_INTEGER t0;
} nu_timer_data_t;

void nu_timer_allocate(nu_timer_t *timer) 
{
    *timer = (nu_timer_t)nu_malloc(sizeof(nu_timer_data_t));
}
void nu_timer_free(nu_timer_t timer) 
{
    nu_free(timer);
}
void nu_timer_start(nu_timer_t timer)
{
    nu_timer_data_t *p = (nu_timer_data_t*)timer;
    QueryPerformanceCounter(&p->t0);
}
float nu_timer_get_time_elapsed(nu_timer_t timer)
{
    nu_timer_data_t *p = (nu_timer_data_t*)timer;

    LARGE_INTEGER frequency, t1;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);

    double delta = (t1.QuadPart - p->t0.QuadPart) * 1000.0 / frequency.QuadPart;
    return (float)delta;
}
#elif defined(NU_PLATFORM_UNIX)
#include <time.h>

typedef struct {
    struct timespec start;
} nu_timer_data_t;

static inline void timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}

void nu_timer_allocate(nu_timer_t *timer) 
{
    *timer = (nu_timer_t)nu_malloc(sizeof(nu_timer_data_t));
}
void nu_timer_free(nu_timer_t timer) 
{
    nu_free(timer);
}
void nu_timer_start(nu_timer_t timer)
{
    nu_timer_data_t *p = (nu_timer_data_t*)timer;
    clock_gettime(CLOCK_MONOTONIC, &p->start);
}
float nu_timer_get_time_elapsed(nu_timer_t timer)
{
    nu_timer_data_t *p = (nu_timer_data_t*)timer;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);

    struct timespec diff;
    timespec_diff(&p->start, &end, &diff);

    return (double)diff.tv_nsec / 1.0e6;
}
#endif