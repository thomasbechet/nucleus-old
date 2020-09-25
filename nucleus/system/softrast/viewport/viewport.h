#ifndef NUSR_VIEWPORT_H
#define NUSR_VIEWPORT_H

#include "../memory/renderbuffer.h"

nu_result_t nusr_viewport_initialize(void);
nu_result_t nusr_viewport_terminate(void);
nu_result_t nusr_viewport_get_renderbuffer(nusr_renderbuffer_t **renderbuffer);
nu_result_t nusr_viewport_set_size(uint32_t width, uint32_t height);
nu_result_t nusr_viewport_get_size(uint32_t *width, uint32_t *height);

#endif