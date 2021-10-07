#pragma once
#include "Core/Declare.h"
#include "Math/Vectors.h"

#include <array>
#include <memory>

namespace Rocket {
    class HalfEdge;

    _Interface_ HalfTriangle {
    public:
        std::shared_ptr<HalfEdge> h;
    };

    using HalfTrianglePtr = std::shared_ptr<HalfTriangle>;
}