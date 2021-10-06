#pragma once
#include "Geometry/Vertex.h"

#include <memory>

namespace Rocket {
    class Edge {
    public:
        Edge() = default;
        ~Edge() = default;
        Edge(const std::shared_ptr<Vertex>& s, const std::shared_ptr<Vertex>& e)
            : start(s), end(e) {}
    public:
        std::shared_ptr<Vertex> start;
        std::shared_ptr<Vertex> end;
    };

    using EdgePtr = std::shared_ptr<Edge>;
}