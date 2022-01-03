#pragma once
#include "Core/MemoryDefine.h"
#include "Common/RenderLoader.h"

namespace Rocket {
    _Interface_ RenderDevice {
    public:
        virtual ~RenderDevice() = default;
        virtual void SetLoader(RenderLoaderPtr loader) = 0;
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
    };

    CLASS_PTR(RenderDevice);
}
