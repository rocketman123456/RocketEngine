#pragma once

#include <memory>

namespace Rocket {
    class RenderAPI {};

    using RenderAPIPtr = std::unique_ptr<RenderAPI>;
}
