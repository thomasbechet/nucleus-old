#include "surface.hpp"

#include "../engine/engine.hpp"
#include "windowinterface.hpp"
#include "surface.hpp"

using namespace nuvk;

namespace
{

}

struct Surface::Internal
{
    VkSurfaceKHR surface;
    const Instance &instance;

    Internal(
        const Instance &instance,
        const WindowInterface &interface
    ) : instance(instance)
    {
        surface = interface.createWindowSurface(instance.getInstance());
    }
    ~Internal()
    {
        vkDestroySurfaceKHR(instance.getInstance(), surface, nullptr);
    }
};

Surface::Surface(
    const Instance &instance,
    const WindowInterface &interface
) : internal(MakeInternalPtr<Internal>(instance, interface)) {}

VkSurfaceKHR Surface::getSurface() const
{
    return internal->surface;
}