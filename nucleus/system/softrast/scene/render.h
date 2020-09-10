#ifndef NUSR_SCENE_RENDER_H
#define NUSR_SCENE_RENDER_H

#include "scene.h"

NU_API nu_result_t nusr_scene_render_global(
    nusr_renderbuffer_t *renderbuffer,
    const nusr_camera_t *camera,
    const nusr_staticmesh_t *staticmeshes,
    uint32_t staticmesh_count
);

#endif