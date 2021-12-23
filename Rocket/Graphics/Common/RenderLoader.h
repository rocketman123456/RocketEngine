#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    _Interface_ RenderLoader {
    public:
        virtual ~RenderLoader() = default;
        virtual void Load() = 0;
        virtual void Unload() = 0;
    };

    CLASS_PTR(RenderLoader);
}
