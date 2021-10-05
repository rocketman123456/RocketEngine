#pragma once
#include "Core/Declare.h"
#include "Math/Vectors.h"

namespace Rocket {
    class RenderEdge;

    _Interface_ RenderVertex {
    public:
        std::shared_ptr<RenderEdge> e;
        Vector3d pos;
    };

    using RenderVertexPtr = std::shared_ptr<RenderVertex>;
}
