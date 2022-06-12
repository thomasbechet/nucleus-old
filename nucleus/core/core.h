#ifndef NU_CORE_H
#define NU_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <nucleus/core/api/module.h>
#include <nucleus/core/api/allocator.h>
#include <nucleus/core/api/config.h>
#include <nucleus/core/api/engine.h>
#include <nucleus/core/api/event.h>
#include <nucleus/core/api/input.h>
#include <nucleus/core/api/logger.h>
#include <nucleus/core/api/renderer.h>
#include <nucleus/core/api/task.h>
#include <nucleus/core/api/window.h>
#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/system/config/api.h>
#include <nucleus/core/system/event/api.h>
#include <nucleus/core/system/module/api.h>
#include <nucleus/core/system/engine/api.h>
#include <nucleus/core/utility/table_printer.h>

NU_API void nu_initialize(void);
NU_API void nu_terminate(void);

#ifdef __cplusplus
}
#endif

#endif