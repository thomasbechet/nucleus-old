#ifndef NUECS_QUERY_H
#define NUECS_QUERY_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_query_create(nuecs_query_t scene);
nu_result_t nuecs_query_destroy(nuecs_query_t query);
nu_result_t nuecs_query_resolve_chunks(nuecs_query_t query, nuecs_query_chunks_t *chunks);

#endif