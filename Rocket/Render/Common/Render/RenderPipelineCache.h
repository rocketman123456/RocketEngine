#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderPipelineCache {
    public:
        virtual ~RenderPipelineCache() = default;
    };

    using RenderPipelineCachePtr = std::unique_ptr<RenderPipelineCache>;
}
