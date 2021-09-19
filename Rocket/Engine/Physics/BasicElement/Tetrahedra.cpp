#include "Physics/BasicElement/Tetrahedra.h"

namespace Rocket {
    Tetrahedra::Tetrahedra() : id_(GenerateId()) {
        faces_ = {
            Triangle(std::vector<Eigen::Vector3d>({p0_, p1_, p2_})),
            Triangle(std::vector<Eigen::Vector3d>({p0_, p2_, p3_})), 
            Triangle(std::vector<Eigen::Vector3d>({p0_, p3_, p1_})), 
            Triangle(std::vector<Eigen::Vector3d>({p1_, p2_, p3_}))
        };
    }

    Tetrahedra::Tetrahedra(const Sphere& sphere) : id_(GenerateId()) {
        p0_ = sphere.Center() + Eigen::Vector3d(0, 0, sphere.Radius() * 2.0);
        p1_ = sphere.Center() + Eigen::Vector3d(sphere.Radius(), std::sqrt(3) * sphere.Radius(), -sphere.Radius());
        p2_ = sphere.Center() + Eigen::Vector3d(sphere.Radius(), -std::sqrt(3) * sphere.Radius(), -sphere.Radius());
        p3_ = sphere.Center() + Eigen::Vector3d(-2 * sphere.Radius(), 0, -sphere.Radius());

        faces_ = {
            Triangle(std::vector<Eigen::Vector3d>({p0_, p1_, p2_})),
            Triangle(std::vector<Eigen::Vector3d>({p0_, p2_, p3_})), 
            Triangle(std::vector<Eigen::Vector3d>({p0_, p3_, p1_})), 
            Triangle(std::vector<Eigen::Vector3d>({p1_, p2_, p3_}))
        };
    }
}
