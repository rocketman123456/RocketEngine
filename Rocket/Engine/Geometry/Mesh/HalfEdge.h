#pragma once
#include "Core/Declare.h"
#include "Math/Vectors.h"

#include <memory>

namespace Rocket {
    class HalfTriangle;
    class HalfVertex;

    _Interface_ HalfEdge {
    public:
        std::shared_ptr<HalfEdge> pair;
        std::shared_ptr<HalfEdge> next;
        std::shared_ptr<HalfEdge> prev;
        std::shared_ptr<HalfTriangle> t;
        std::shared_ptr<HalfVertex> v;
        int32_t i;
    };

    using HalfEdgePtr = std::shared_ptr<HalfEdge>;
}
