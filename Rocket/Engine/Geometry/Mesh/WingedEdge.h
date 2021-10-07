#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class WingedVertex;
    class WingedTriangle;

    class WingedEdge {
    public:
        std::shared_ptr<WingedVertex> head, tail;
        std::shared_ptr<WingedTriangle> left, right;
        std::shared_ptr<WingedEdge> lnext, lprev, rnext, rprev;
    };

    using WingedEdgePtr = std::shared_ptr<WingedEdge>;
}
