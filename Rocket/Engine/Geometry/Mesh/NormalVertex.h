#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Math/Vectors.h"

namespace Rocket {
    class NormalEdge;

    _Interface_ NormalVertex {
    public:
        std::shared_ptr<NormalEdge> e;
        Vector3d pos;
    };

    using NormalVertexPtr = std::shared_ptr<NormalVertex>;
}
