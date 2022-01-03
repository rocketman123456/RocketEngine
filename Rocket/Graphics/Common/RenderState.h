#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    _Interface_ RenderState {
    public:
        virtual ~RenderState() = default;
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
    };
}
