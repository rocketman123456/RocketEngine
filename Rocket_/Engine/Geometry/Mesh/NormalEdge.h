#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Math/Vectors.h"

#include <memory>

namespace Rocket {
    class NormalTriangle;

    _Interface_ NormalEdge {
    public:
        std::shared_ptr<NormalTriangle> t;
        int32_t i;
    };

    using NormalEdgePtr = std::shared_ptr<NormalEdge>;
}
