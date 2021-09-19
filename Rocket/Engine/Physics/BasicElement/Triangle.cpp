#include "Physics/BasicElement/Triangle.h"

namespace Rocket {
    Triangle::Triangle() : id_(GenerateId()) {
        vertices_ = {
            Eigen::Vector3d::Zero(),
            Eigen::Vector3d::Zero(),
            Eigen::Vector3d::Zero()
        };
        edges_ = {
            std::make_pair(vertices_[0], vertices_[1]),
            std::make_pair(vertices_[1], vertices_[2]),
            std::make_pair(vertices_[2], vertices_[0])
        };
    }

    Triangle::Triangle(const std::array<Eigen::Vector3d, 3>& vertices) : id_(GenerateId()), vertices_(vertices) {
        edges_ = {
            std::make_pair(vertices_[0], vertices_[1]),
            std::make_pair(vertices_[1], vertices_[2]),
            std::make_pair(vertices_[2], vertices_[0])
        };
    }

    Triangle::Triangle(const std::vector<Eigen::Vector3d>& vertices) : id_(GenerateId()) {
        vertices_ = {
            vertices[0], vertices[1], vertices[2]
        };
        edges_ = {
            std::make_pair(vertices_[0], vertices_[1]),
            std::make_pair(vertices_[1], vertices_[2]),
            std::make_pair(vertices_[2], vertices_[0])
        };
    }
}
