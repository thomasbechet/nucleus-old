#pragma once

#include "../utility/internalptr.hpp"

namespace nuvk
{
    class Engine
    {
    public:
        Engine();

        void render();
        void initialize();
        void terminate();

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