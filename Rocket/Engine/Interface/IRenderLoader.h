#pragma once
#include "Core/Core.h"

namespace Rocket {
    class IRenderLoader {
    public:
        virtual ~IRenderLoader() = default;

        [[nodiscard]] virtual int32_t LoadDriver() = 0;
        virtual void UnloadDriver() = 0;
        [[nodiscard]] virtual int32_t LoadDevice() = 0;
        virtual void UnloadDevice() = 0;
        [[nodiscard]] virtual int32_t LoadEngine() = 0;
        virtual void UnloadEngine() = 0;
    };
}