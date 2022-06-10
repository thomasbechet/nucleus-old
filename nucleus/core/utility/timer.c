#include <nucleus/core/utility/timer.h>

#include <nucleus/core/system/allocator/api.h>

#if defined(NU_PLATFORM_WINDOWS)
#include <Windows.h>

typedef struct {
    nu_allocator_t allocator;
    LARGE_INTEGER t0;
} nu_timer_data_t;

nu_timer_t nu_timer_allocate(nu_allocator_t allocator) 
{
    nu_timer_data_t *data = nu_malloc(allocator, sizeof(*data));
    data->allocator = allocator;
    return (nu_timer_t)data;
}
void nu_timer_free(nu_timer_t timer) 
{
    nu_timer_data_t *data = (nu_timer_data_t*)timer;
    nu_free(data->allocator, data);
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
    nu_allocator_t allocator;
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

nu_timer_t nu_timer_allocate(nu_allocator_t allocator) 
{
    nu_timer_data_t *data = nu_malloc(allocator, sizeof(*data));
    data->allocator = allocator;
    return (nu_timer_t)data;
}
void nu_timer_free(nu_timer_t timer) 
{
    nu_timer_data_t *data = (nu_timer_data_t*)timer;
    nu_free(data->allocator, data);
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