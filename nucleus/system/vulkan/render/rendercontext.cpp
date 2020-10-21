#include "rendercontext.hpp"

#include "renderpass.hpp"

using namespace nuvk;

namespace
{

}

struct RenderContext::Internal
{
    std::unique_ptr<RenderPass> renderPass;

    Internal(Context &context)
    {
        renderPass = std::make_unique<RenderPass>(
            context.getDevice().getDevice(),
            context.getSwapchain().getFormat()
        );
    }
    ~Internal()
    {

    }
};

RenderContext::RenderContext(Context &context) :
    internal(MakeInternalPtr<Internal>(context)) {}