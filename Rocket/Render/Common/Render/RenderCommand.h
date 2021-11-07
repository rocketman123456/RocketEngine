#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderCommand {
    public:
        virtual ~RenderCommand() = default;
    };

    using RenderCommandPtr = std::unique_ptr<RenderCommand>;
}
