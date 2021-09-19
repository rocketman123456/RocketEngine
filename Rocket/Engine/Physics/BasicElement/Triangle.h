#pragma once
#include <Eigen/Eigen>
#include <array>
#include <vector>
#include <atomic>

namespace Rocket {
    class Triangle {
    public:
        Triangle();
        Triangle(const std::array<Eigen::Vector3d, 3>& vertices);
        Triangle(const std::vector<Eigen::Vector3d>& vertices);
        ~Triangle() = default;

        inline std::array<Eigen::Vector3d, 3>& Vertices() { return vertices_; }
        inline std::array<std::pair<Eigen::Vector3d, Eigen::Vector3d>, 3>& Edges() { return edges_; }
        inline const std::array<Eigen::Vector3d, 3>& Vertices() const { return vertices_; }
        inline const std::array<std::pair<Eigen::Vector3d, Eigen::Vector3d>, 3>& Edges() const { return edges_; }

    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    private:
        int32_t id_ = 0;
        std::array<Eigen::Vector3d, 3> vertices_;
        std::array<std::pair<Eigen::Vector3d, Eigen::Vector3d>, 3> edges_;
    };
}
