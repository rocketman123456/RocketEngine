#pragma once
#include "Interface/IRenderLoader.h"

namespace Rocket {
    class SoftRenderLoader {
    public:
        virtual ~SoftRenderLoader() = default;

        virtual int32_t LoadDriver() final;
        virtual void UnloadDriver() final;
        virtual int32_t LoadDevice() final;
        virtual void UnloadDevice() final;
        virtual int32_t LoadEngine() final;
        virtual void UnloadEngine() final;
    };
}
