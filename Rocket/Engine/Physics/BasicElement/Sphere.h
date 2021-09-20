#pragma once
#include "Physics/BasicElement/Vertex.h"

#include <Eigen/Eigen>
#include <vector>
#include <atomic>
#include <list>

namespace Rocket {
    class Sphere {
        using VertexList = std::list<Vertex>;
        using VertexListIter = std::list<Vertex>::iterator;
    public:
        Sphere() : id(GenerateId()) {}
        Sphere(const Eigen::Vector3d& center, double radius);
        Sphere(const std::vector<Vertex>& vertices);
        ~Sphere() = default;

        void CreateBoundingSphere(const std::vector<Vertex>& vertices);
        void GenerateMesh();

    private:
        void CreateSphere(const std::vector<Vertex>& vertices, int32_t nsp);
        void CreateSphere();
        void CreateSphere(const Vertex& p1);
        void CreateSphere(const Vertex& p1, const Vertex& p2);
        void CreateSphere(const Vertex& p1, const Vertex& p2, const Vertex& p3);
        void CreateSphere(const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4);

        bool IsOut(const Vertex& point);
        void SmallestEnclosingSphere(std::vector<Vertex>& support, int32_t nsp, VertexList& vertex_list, const VertexListIter& end);

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
}
