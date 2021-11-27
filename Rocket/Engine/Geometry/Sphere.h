#pragma once
#include "Memory/MemoryDefine.h"
#include "Geometry/Vertex.h"
#include "Geometry/Triangle.h"

#include <Eigen/Eigen>
#include <memory>
#include <vector>
#include <list>

namespace Rocket {
    namespace Geometry {
        class Sphere {
            using VertexList = std::list<VertexPtr>;
            using VertexListIter = std::list<VertexPtr>::iterator;
        public:
            Sphere() : id(GenerateId()) {}
            Sphere(const Eigen::Vector3d& center, double radius);
            Sphere(const std::vector<VertexPtr>& vertices);
            ~Sphere() = default;

            // TODO : https://www.redblobgames.com/x/1938-square-tiling-of-sphere/
            void CreateBoundingSphere(const std::vector<VertexPtr>& vertices);
            void GenerateUVMesh(int parallels, int meridians);
            void GenerateIcosahedronMesh(int recursionLevel);

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
            std::vector<TrianglePtr> mesh;
        };

        using SpherePtr = std::shared_ptr<Sphere>;
    }
}
