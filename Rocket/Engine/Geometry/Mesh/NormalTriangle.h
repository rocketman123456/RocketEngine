#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Math/Vectors.h"
#include "Geometry/Mesh/NormalVertex.h"
#include "Geometry/Mesh/NormalEdge.h"

#include <array>
#include <memory>

namespace Rocket {
    _Interface_ NormalTriangle {
    public:
        std::array<NormalVertexPtr, 3> v;
        std::array<NormalEdgePtr, 3> nbr;
    };

    using NormalTrianglePtr = std::shared_ptr<NormalTriangle>;
}