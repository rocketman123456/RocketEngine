#pragma once
#include "Memory/MemoryDefine.h"
#include "Geometry/Sphere.h"
#include "Geometry/Triangle.h"

#include <Eigen/Eigen>
#include <vector>
#include <array>
#include <atomic>

namespace Rocket {
    namespace Geometry {
        class Tetrahedra {
        public:
            Tetrahedra();
            Tetrahedra(const SpherePtr& sphere);
            Tetrahedra(const VertexPtr& p0, const VertexPtr& p1, const VertexPtr& p2, const VertexPtr& p3);

            void GenerateFaces();
            void UpdateFaces();
            void GenerateBasicParameter(
                const Eigen::Vector3d& p0, 
                const Eigen::Vector3d& p1, 
                const Eigen::Vector3d& p2, 
                const Eigen::Vector3d& p3);

            void CreateBoundingTetrahedra(const SpherePtr& sphere);
            void CreateBoundingTetrahedra(const SpherePtr& sphere, const Eigen::Vector3d& x, const Eigen::Vector3d& y, const Eigen::Vector3d& z);

            Tetrahedra* GetLocateId(VertexPtr& v);
            TrianglePtr GetAdjacentSurface(Tetrahedra* t);
            bool IsInSphere(VertexPtr& v);

        private:
            static int32_t GenerateId() {
                static std::atomic<int32_t> id = 0;
                return id++;
            }

        public:
            // TODO : remove copy when possible
            int32_t id = 0;
            std::array<VertexPtr, 4> nodes = {};
            std::array<TrianglePtr, 4> faces = {};
            VertexPtr scenter;
            VertexPtr gcenter;
            double sround;
            double volume;
            double aspect;
            bool is_active = false;
        };

        using TetrahedraPtr = std::shared_ptr<Tetrahedra>;
    }
}
