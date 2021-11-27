#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Math/Vectors.h"

namespace Rocket {
    class HalfEdge;

    _Interface_ HalfVertex {
    public:
        std::shared_ptr<HalfEdge> h;
        Vector3d pos;
    };

    using HalfVertexPtr = std::shared_ptr<HalfVertex>;
}
