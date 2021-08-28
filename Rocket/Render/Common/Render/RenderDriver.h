#pragma once

#include <memory>

namespace Rocket {
    class RenderDriver {};

    using RenderDriverPtr = std::unique_ptr<RenderDriver>;
}
