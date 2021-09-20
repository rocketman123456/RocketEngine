#pragma once
#include "Physics/BasicElement/Vertex.h"

#include <Eigen/Eigen>
#include <array>
#include <vector>
#include <atomic>

namespace Rocket {
    class Tetrahedra;
    using Edge = std::pair<Vertex, Vertex>;

    class Triangle {
    public:
        Triangle();
        Triangle(const std::vector<Vertex>& vertices);
        Triangle(const std::vector<Vertex>& vertices, std::shared_ptr<Tetrahedra>& parent, std::shared_ptr<Tetrahedra>& neighbor);
        ~Triangle() = default;

        bool IsCoincidentWith(const Vertex& v);

        bool operator==(const Triangle& t);
        bool IsRayCross(Vertex a, Vertex b);

        void UpdateFaces();
    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    public:
        // TODO : remove copy when possible
        int32_t id = 0;
        std::array<Vertex, 3> vertices = {};
        std::array<Edge, 3> edges = {};
        std::shared_ptr<Tetrahedra> parent = nullptr;
        std::shared_ptr<Tetrahedra> neighbor = nullptr;
        bool is_active = false;
    };
}
