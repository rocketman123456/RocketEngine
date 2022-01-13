#pragma once
#include "Core/MemoryDefine.h"
#include "Common/RenderLoader.h"

namespace Rocket {
    _Interface_ RenderDeviceLoader {
    public:
        virtual ~RenderDeviceLoader() = default;
        virtual void SetLoader(RenderLoaderPtr loader) = 0;
        virtual void SetSize(uint32_t width, uint32_t height) = 0;
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
    };

    CLASS_PTR(RenderDeviceLoader);
}
