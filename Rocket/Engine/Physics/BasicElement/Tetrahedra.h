#pragma once
#include "Physics/BasicElement/Sphere.h"
#include "Physics/BasicElement/Triangle.h"

#include <Eigen/Eigen>
#include <vector>
#include <array>
#include <atomic>

namespace Rocket {
    class Tetrahedra {
    public:
        Tetrahedra();
        Tetrahedra(const Sphere& sphere);

        inline const Eigen::Vector3d& P0() const { return p0_; }
        inline const Eigen::Vector3d& P1() const { return p1_; }
        inline const Eigen::Vector3d& P2() const { return p2_; }
        inline const Eigen::Vector3d& P3() const { return p3_; }
        inline Eigen::Vector3d& P0() { return p0_; }
        inline Eigen::Vector3d& P1() { return p1_; }
        inline Eigen::Vector3d& P2() { return p2_; }
        inline Eigen::Vector3d& P3() { return p3_; }
        inline const std::array<Triangle, 4>& Faces() const { return faces_; }
        inline std::array<Triangle, 4>& Faces() { return faces_; }

    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    private:
        int32_t id_ = 0;
        Eigen::Vector3d p0_ = Eigen::Vector3d::Zero();
        Eigen::Vector3d p1_ = Eigen::Vector3d::Zero();
        Eigen::Vector3d p2_ = Eigen::Vector3d::Zero();
        Eigen::Vector3d p3_ = Eigen::Vector3d::Zero();
        std::array<Triangle, 4> faces_;
    };
}
