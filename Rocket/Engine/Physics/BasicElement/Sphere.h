#pragma once
#include "Physics/BasicElement/Vertex.h"

#include <Eigen/Eigen>
#include <vector>
#include <atomic>

namespace Rocket {
    class Sphere {
    public:
        Sphere() : id_(GenerateId()) {}
        Sphere(const Eigen::Vector3d& center, double radius);
        Sphere(const std::vector<Vertex>& vertices);
        ~Sphere() = default;

        inline Eigen::Vector3d& Center() { return center_; }
        inline const Eigen::Vector3d& Center() const { return center_; }
        inline double& Radius() { return radius_; }
        inline double Radius() const { return radius_; }

    private:
        void CreateSphere(const std::vector<Vertex>& vertices);
        void CreateSphere();
        void CreateSphere(const Vertex& p1);
        void CreateSphere(const Vertex& p1, const Vertex& p2);
        void CreateSphere(const Vertex& p1, const Vertex& p2, const Vertex& p3);
        void CreateSphere(const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4);

        void CreateBoundingSphere(const std::vector<Vertex>& vertices);
        void SmallestEnclosingSphere();

        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    private:
        int32_t id_ = 0;
        Eigen::Vector3d center_ = Eigen::Vector3d::Zero();
        double radius_ = 0.0;
    };
}
