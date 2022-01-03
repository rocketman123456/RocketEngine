#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    _Interface_ RenderLoader {
    public:
        virtual ~RenderLoader() = default;
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void* GetWindow() = 0;
        virtual void SetWindow(void* window) = 0;
    };

    CLASS_PTR(RenderLoader);
}
