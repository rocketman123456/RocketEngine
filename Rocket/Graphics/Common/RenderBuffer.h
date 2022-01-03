#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    enum class BufferType {
        UNIFORM = 0,
        TARGET,
    };

    _Interface_ RenderBuffer {
    public:
        virtual ~RenderBuffer() = default;
    };
}
