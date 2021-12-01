#pragma once
#include "Memory/MemoryDefine.h"
#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"

#include <Eigen/Eigen>
#include <array>
#include <vector>
#include <atomic>
#include <memory>

namespace Rocket {
    namespace Geometry {
        class Tetrahedra;

        class Triangle {
        public:
            Triangle();
            virtual ~Triangle() = default;
            Triangle(VertexPtr v1, VertexPtr v2, VertexPtr v3);
            Triangle(const std::vector<VertexPtr>& vertices);
            Triangle(const std::vector<VertexPtr>& vertices, Tetrahedra* parent, Tetrahedra* neighbor);

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
            int32_t id = 0;
            std::array<VertexPtr, 3> vertices = {};
            std::array<EdgePtr, 3> edges = {};
            Tetrahedra* parent = nullptr;
            Tetrahedra* neighbor = nullptr;
            bool is_active = false;
        };

        using TrianglePtr = std::shared_ptr<Triangle>;
    }
}
