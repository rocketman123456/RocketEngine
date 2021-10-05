#pragma once
#include "Physics/Basic/Vertex.h"
#include "Physics/Basic/Edge.h"

#include <Eigen/Eigen>
#include <array>
#include <vector>
#include <atomic>
#include <memory>

namespace Rocket {
    class Tetrahedra;
    //using Edge = std::pair<std::shared_ptr<Vertex>, std::shared_ptr<Vertex>>;

    class Triangle {
    public:
        Triangle();
        Triangle(const std::vector<VertexPtr>& vertices);
        Triangle(const std::vector<VertexPtr>& vertices, Tetrahedra* parent, Tetrahedra* neighbor);
        ~Triangle() = default;

        bool IsCoincidentWith(const VertexPtr& v);
        bool IsCoincidentWith(Triangle* t);
        bool IsRayCross(VertexPtr& a, VertexPtr& b);

        void UpdateEdges();
    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    public:
        // TODO : remove copy when possible
        int32_t id = 0;
        std::array<VertexPtr, 3> vertices = {};
        std::array<EdgePtr, 3> edges = {};
        Tetrahedra* parent = nullptr;
        Tetrahedra* neighbor = nullptr;
        bool is_active = false;
    };

    using TrianglePtr = std::shared_ptr<Triangle>;
}
