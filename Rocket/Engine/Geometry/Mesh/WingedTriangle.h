#pragma once
#include "Core/Declare.h"
#include "Geometry/Mesh/WingedEdge.h"

#include <memory>

namespace Rocket {
    class WingedTriangle {
    public:
        WingedEdgePtr e;
    };

    using WingedTrianglePtr = std::shared_ptr<WingedTriangle>;
}
