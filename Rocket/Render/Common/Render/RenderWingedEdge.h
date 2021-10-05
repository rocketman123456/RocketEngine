#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderWingedVertex;
    class RenderWingedTriangle;

    class RenderWingedEdge {
    public:
        std::shared_ptr<RenderWingedVertex> head, tail;
        std::shared_ptr<RenderWingedTriangle> left, right;
        std::shared_ptr<RenderWingedEdge> lnext, lprev, rnext, rprev;
    };

    using RenderWingedEdgePtr = std::shared_ptr<RenderWingedEdge>;
}
