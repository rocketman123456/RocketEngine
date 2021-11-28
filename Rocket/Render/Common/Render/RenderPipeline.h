#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderPipeline {
    public:
        virtual ~RenderPipeline() = default;
    };

    using RenderPipelinePtr = std::unique_ptr<RenderPipeline>;
}
