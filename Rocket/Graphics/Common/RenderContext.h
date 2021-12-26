#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    _Interface_ RenderContext {
    public:
        virtual ~RenderContext() = default;
        virtual void BeginRenderPass() = 0;
        virtual void EndRenderPass() = 0;
    };
}
