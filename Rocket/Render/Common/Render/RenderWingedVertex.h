#pragma once
#include "Core/Declare.h"
#include "Render/RenderWingedEdge.h"

#include <memory>

namespace Rocket {
    class RenderWingedVertex {
    public:
        RenderWingedEdgePtr e;
    };

    using RenderWingedVertexPtr = std::shared_ptr<RenderWingedVertex>;
}
