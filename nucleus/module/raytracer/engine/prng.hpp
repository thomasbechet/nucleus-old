#pragma once

#include <random>

namespace nurt
{
    class PRNG
    {
    public:
        PRNG(long seed);

        float getFloat();
        int getInteger();
        unsigned getUnsigned();

    private:
        long m_seed;
    };
}