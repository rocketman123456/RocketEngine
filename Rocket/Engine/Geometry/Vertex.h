#pragma once
#include <Eigen/Eigen>
#include <atomic>
#include <memory>

namespace Rocket {
    namespace Geometry {
        class Vertex {
        public:
            Vertex() : id(GenerateId()) {}
            Vertex(double x, double y, double z, int32_t type = 0) 
                : id(GenerateId()), type(type), position(x, y, z) {}
            Vertex(const Eigen::Vector3d& pos, int32_t type = 0) 
                : id(GenerateId()), type(type), position(pos) {}
            ~Vertex() = default;

            bool IsCoincidentWith(const Vertex* v);

        private:
            static int32_t GenerateId() {
                static std::atomic<int32_t> id = 0;
                return id++;
            }

        public:
            int32_t id = 0;
            int32_t type = 0;
            Eigen::Vector3d position = Eigen::Vector3d::Zero();
            bool is_auxiliary = false;
            bool is_visited = false;
            bool is_active = false;
        };

        using VertexPtr = std::shared_ptr<Vertex>;
        bool CompareVertex(const VertexPtr& lhs, const VertexPtr& rhs);
    }
}
