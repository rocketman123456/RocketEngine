#pragma once

#include <memory>

namespace Rocket {
    enum class BufferType : uint32_t {
        COLOR = 0,
        DEPTH,
        UNKNOWN,
    };

    class RenderBuffer {
    public:
        virtual ~RenderBuffer() = default;
    };

    using RenderBufferPtr = std::unique_ptr<RenderBuffer>;
}
