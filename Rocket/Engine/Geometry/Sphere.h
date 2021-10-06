#pragma once
#include "Geometry/Vertex.h"

#include <Eigen/Eigen>
#include <vector>
#include <atomic>
#include <list>
#include <memory>

namespace Rocket {
    class Sphere {
        using VertexList = std::list<VertexPtr>;
        using VertexListIter = std::list<VertexPtr>::iterator;
    public:
        Sphere() : id(GenerateId()) {}
        Sphere(const Eigen::Vector3d& center, double radius);
        Sphere(const std::vector<VertexPtr>& vertices);
        ~Sphere() = default;

        void CreateBoundingSphere(const std::vector<VertexPtr>& vertices);
        void GenerateMesh();

    private:
        void CreateSphere(const std::vector<VertexPtr>& vertices, int32_t nsp);
        void CreateSphere();
        void CreateSphere(const VertexPtr& p1);
        void CreateSphere(const VertexPtr& p1, const VertexPtr& p2);
        void CreateSphere(const VertexPtr& p1, const VertexPtr& p2, const VertexPtr& p3);
        void CreateSphere(const VertexPtr& p1, const VertexPtr& p2, const VertexPtr& p3, const VertexPtr& p4);

        bool IsOut(const VertexPtr& point);
        void SmallestEnclosingSphere(std::vector<VertexPtr>& support, int32_t nsp, VertexList& vertex_list, const VertexListIter& end);

        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    public:
        int32_t id = 0;
        Eigen::Vector3d center = Eigen::Vector3d::Zero();
        double radius = 0.0;
        bool is_active = false;
    };

    using SpherePtr = std::shared_ptr<Sphere>;
}
