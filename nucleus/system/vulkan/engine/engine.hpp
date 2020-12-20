#pragma once

#include "../utility/internalptr.hpp"

#include <nucleus/nucleus.h>

namespace nuvk
{
    class Engine
    {
    public:
        Engine();

        void render();

    public:
        void createMesh(const nu_renderer_mesh_create_info_t &info);

    public:
        static inline constexpr std::string_view Section = "ENGINE";

        static void Interrupt(std::string_view section, std::string_view msg);

    private:
        void drawFrame();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}