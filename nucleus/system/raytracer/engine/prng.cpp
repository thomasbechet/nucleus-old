#include "prng.hpp"

using namespace nurt;

PRNG::PRNG(long seed) :
    m_seed(seed)
{

}

float PRNG::getFloat()
{
    unsigned a = getUnsigned();
    unsigned b = getUnsigned();
    return (a > b) ? (float)b / (float)a : (float)a / (float)b;
}
int PRNG::getInteger()
{
    return getUnsigned();
}
unsigned PRNG::getUnsigned()
{
    m_seed = (8253729 * m_seed + 2396403);
    return m_seed  % 4294967295;
}