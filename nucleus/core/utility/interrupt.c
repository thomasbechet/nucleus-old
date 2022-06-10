#include <nucleus/core/utility/interrupt.h>

#include <nucleus/core/utility/result.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>

void nu_interrupt_(const char *file, uint64_t line)
{
    nu_fatal(nu_logger_get_core(), "Interruption (%s:%d)", file, line);
    exit(NU_FAILURE);
}