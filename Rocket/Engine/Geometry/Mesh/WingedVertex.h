#pragma once
#include "Core/Declare.h"
#include "Geometry/Mesh/WingedEdge.h"

#include <memory>

namespace Rocket {
    class WingedVertex {
    public:
        WingedEdgePtr e;
    };

    using WingedVertexPtr = std::shared_ptr<WingedVertex>;
}
