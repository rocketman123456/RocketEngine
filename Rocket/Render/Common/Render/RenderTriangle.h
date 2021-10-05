#pragma once
#include "Core/Declare.h"
#include "Math/Vectors.h"
#include "Render/RenderVertex.h"
#include "Render/RenderEdge.h"

#include <array>
#include <memory>

namespace Rocket {
    _Interface_ RenderTriangle {
    public:
        std::array<RenderVertexPtr, 3> v;
        std::array<RenderEdgePtr, 3> nbr;
    };

    using RenderTrianglePtr = std::shared_ptr<RenderTriangle>;
}