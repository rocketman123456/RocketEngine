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
        Tetrahedra(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3);

        void CreateBoundingTetrahedra(const Sphere& sphere);
        void CreateBoundingTetrahedra(const Sphere& sphere, const Eigen::Vector3d& x, const Eigen::Vector3d& y, const Eigen::Vector3d& z);
        void UpdateFaces();
        void GenerateBasicParameter(
            const Eigen::Vector3d& p0, 
            const Eigen::Vector3d& p1, 
            const Eigen::Vector3d& p2, 
            const Eigen::Vector3d& p3);

        std::shared_ptr<Tetrahedra> GetLocateId(Vertex& v);
		bool IsInSphere(Vertex& v);
		Triangle* GetAdjacentSurface(std::shared_ptr<Tetrahedra> t);

        bool operator==(const Tetrahedra& t);

    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    public:
        // TODO : remove copy when possible
        int32_t id = 0;
        std::array<Vertex, 4> nodes = {};
        std::array<Triangle, 4> faces = {};
        Vertex scenter;
		Vertex gcenter;
		double sround;
		double volume;
		double aspect;
        bool is_active = false;
    };
}
