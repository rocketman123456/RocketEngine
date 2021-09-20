#include "Physics/BasicElement/Tetrahedra.h"
#include "Physics/BasicElement/Parameter.h"

#include <fenv.h>

namespace Rocket {
    Tetrahedra::Tetrahedra() : id(GenerateId()) {
        UpdateFaces();
    }

    Tetrahedra::Tetrahedra(const Sphere& sphere) : id(GenerateId()) {
        CreateBoundingTetrahedra(sphere);
    }

    void Tetrahedra::CreateBoundingTetrahedra(const Sphere& sphere) {
        auto x = Eigen::Vector3d(0, 0, 1);
        auto y = Eigen::Vector3d(1, 0, 0);
        auto z = Eigen::Vector3d(0, 1, 0);
        
        CreateBoundingTetrahedra(sphere, x, y, z);
    }

    void Tetrahedra::CreateBoundingTetrahedra(
        const Sphere& sphere, 
        const Eigen::Vector3d& x, 
        const Eigen::Vector3d& y, 
        const Eigen::Vector3d& z) {
        
        double r = sphere.radius;
        nodes[0].position = sphere.center + z * r * 3.0;
        nodes[1].position = sphere.center + z * r * -1.0 + x * 2.0 * r + y * std::sqrt(6) * r;
        nodes[2].position = sphere.center + z * r * -1.0 + x * 2.0 * r - y * std::sqrt(6) * r;
        nodes[3].position = sphere.center + z * r * -1.0 - x * 4.0 * r;

        Eigen::Vector3d p0 = nodes[0].position;
        Eigen::Vector3d p1 = nodes[1].position;
        Eigen::Vector3d p2 = nodes[2].position;
        Eigen::Vector3d p3 = nodes[3].position;
        
        UpdateFaces();
        GenerateBasicParameter(p0, p1, p2, p3);
    }

    Tetrahedra::Tetrahedra(const Vertex& p0_, const Vertex& p1_, const Vertex& p2_, const Vertex& p3_) : id(GenerateId()) {
        nodes[0] = (p0_);
        nodes[1] = (p1_);
        nodes[2] = (p2_);
        nodes[3] = (p3_);

        Eigen::Vector3d p0 = p0_.position;
        Eigen::Vector3d p1 = p1_.position;
        Eigen::Vector3d p2 = p2_.position;
        Eigen::Vector3d p3 = p3_.position;

        UpdateFaces();
        GenerateBasicParameter(p0, p1, p2, p3);
    }

    void Tetrahedra::UpdateFaces() {
        Eigen::Vector3d p0 = nodes[0].position;
        Eigen::Vector3d p1 = nodes[1].position;
        Eigen::Vector3d p2 = nodes[2].position;
        Eigen::Vector3d p3 = nodes[3].position;

        faces = {
            Triangle(std::vector<Vertex>({p0, p1, p2})),
            Triangle(std::vector<Vertex>({p0, p2, p3})), 
            Triangle(std::vector<Vertex>({p0, p3, p1})), 
            Triangle(std::vector<Vertex>({p1, p2, p3}))
        };
    }

    void Tetrahedra::GenerateBasicParameter(
        const Eigen::Vector3d& p0, 
        const Eigen::Vector3d& p1, 
        const Eigen::Vector3d& p2, 
        const Eigen::Vector3d& p3) {
        //----------Get center and radius of external sphere----------
		Eigen::Vector3d v0 = p1 - p0;
		Eigen::Vector3d v1 = p2 - p0;
		Eigen::Vector3d v2 = p3 - p0;

		Eigen::Vector3d ABC = Eigen::Vector3d(
            0.5*((p1.dot(p1)) - (p0.dot(p0))), 
            0.5*((p2.dot(p2)) - (p0.dot(p0))), 
            0.5*((p3.dot(p3)) - (p0.dot(p0)))
        );

		double detP = v0.dot(v1.cross(v2));
		Eigen::Vector3d P0 = v1.cross(v2);
		Eigen::Vector3d P1 = v2.cross(v0);
		Eigen::Vector3d P2 = v0.cross(v1);

        int32_t x = 0, y = 1, z = 2;
		this->scenter = Eigen::Vector3d(
            (ABC[x] * P0[x] + ABC[y] * P1[x] + ABC[z] * P2[x]) / detP, 
            (ABC[x] * P0[y] + ABC[y] * P1[y] + ABC[z] * P2[y]) / detP, 
            (ABC[x] * P0[z] + ABC[y] * P1[z] + ABC[z] * P2[z]) / detP
        );
		this->sround = (this->scenter.position - p0).norm();

		//----------Get center of gravity----------
		this->gcenter.position = (p0 + p1 + p2 + p3) / 4.0;

		//----------Get volume----------
		this->volume = ((p1 - p0).cross(p2 - p0)).dot(p3 - p0);
		
		//----------Get aspect ratio----------
		this->aspect = this->volume / pow(this->sround, 3.0) / ARTETRAHEDRON;
		if (fetestexcept(FE_DIVBYZERO)) {
			feclearexcept(FE_ALL_EXCEPT);
			this->aspect = 0.0;
		}
    }

    bool Tetrahedra::operator==(const Tetrahedra& t) {
        return t.id == id;
    }

    std::shared_ptr<Tetrahedra> Tetrahedra::GetLocateId(Vertex& v) {
        for (auto surface : this->faces) {
            Vertex v_;
            v_.position = this->gcenter.position - v.position;
			if (surface.IsRayCross(this->gcenter, v_)) {
				return surface.neighbor;
			}
		}
        auto this_copy = std::make_shared<Tetrahedra>();
        *this_copy = *this;
		return this_copy;
    }

    bool Tetrahedra::IsInSphere(Vertex& v) {
        Eigen::Vector3d dt = this->scenter.position - v.position;
        if ((sround + EPS) * (sround + EPS) > dt.squaredNorm()) {
			return true;
		}
		return false;
    }

    Triangle* Tetrahedra::GetAdjacentSurface(std::shared_ptr<Tetrahedra> t) {
        for (auto& surface : this->faces) {
			if (*surface.neighbor.get() == *t.get()) {
				return &surface;
			}
		}
        return nullptr;
    }
}
