#include "Physics/BasicElement/Sphere.h"
#include "Physics/BasicElement/Parameter.h"
#include "Log/Log.h"

#include <algorithm>
#include <cstdlib>
#include <random>
#include <iostream>

namespace Rocket {
    Sphere::Sphere(const Eigen::Vector3d& center, double radius) 
        : id(GenerateId()), center(center), radius(radius) {}

    Sphere::Sphere(const std::vector<Vertex>& vertices) : id(GenerateId()) {
        if(vertices.size() == 0) 
            return;
        else {
            CreateBoundingSphere(vertices);
        }
    }

    void Sphere::CreateSphere(const std::vector<Vertex>& vertices, int32_t nsp) {
        switch(nsp) {
            case 0:
                CreateSphere();
                break;
            case 1:
                CreateSphere(vertices[0]);
                break;
            case 2:
                CreateSphere(vertices[0], vertices[1]);
                break;
            case 3:
                CreateSphere(vertices[0], vertices[1], vertices[2]);
                break;
            case 4:
                CreateSphere(vertices[0], vertices[1], vertices[2], vertices[3]);
                break;
        }
    }

    void Sphere::CreateSphere() {
        center = Eigen::Vector3d::Zero();
        radius = 0.0;
    }

    void Sphere::CreateSphere(const Vertex& p1) {
        center = p1.position;
        radius = 0.0;
    }

    void Sphere::CreateSphere(const Vertex& p1, const Vertex& p2) {
        center = (p1.position + p2.position) / 2;
		radius = ((p1.position - p2.position) / 2.0).norm();
    }

    void Sphere::CreateSphere(const Vertex& p0, const Vertex& p1, const Vertex& p2) {
        Eigen::Vector3d e0 = p1.position - p0.position;
        Eigen::Vector3d e1 = p2.position - p0.position;
		double a = e0.dot(e0);
        double b = e0.dot(e1);
        double c = e1.dot(e1);
		double	d = a * c - b * b;
        if(std::abs(d) < EPS)
			return;
		double s = (a - b)*c / (2 * d), t = (c - b)*a / (2 * d);
		center = p0.position + s * e0 + t * e1;
		radius = (p0.position - center).norm();
    }

    void Sphere::CreateSphere(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3) {
        Eigen::Vector3d v1 = p1.position - p0.position;
        Eigen::Vector3d v2 = p2.position - p0.position;
        Eigen::Vector3d v3 = p3.position - p0.position;
		double V = v1.dot(v2.cross(v3));
		//Check that the three points are not on the same plane.
        if(std::abs(V) < EPS)
			return;
		V *= 2.0;
		double L1 = v1.squaredNorm();
        double L2 = v2.squaredNorm();
        double L3 = v3.squaredNorm();
		center[0] = (p0.position[0] + ((v2[1]*v3[2] - v3[1]*v2[2])*L1 - (v1[1]*v3[2] - v3[1]*v1[2])*L2 + (v1[1]*v2[2] - v2[1]*v1[2])*L3) / V);
		center[1] = (p0.position[1] + (-(v2[0]*v3[2] - v3[0]*v2[2])*L1 + (v1[0]*v3[2] - v3[0]*v1[2])*L2 - (v1[0]*v2[2] - v2[0]*v1[2])*L3) / V);
		center[2] = (p0.position[2] + ((v2[0]*v3[1] - v3[0]*v2[1])*L1 - (v1[0]*v3[1] - v3[0]*v1[1])*L2 + (v1[0]*v2[1] - v2[0]*v1[1])*L3) / V);
		radius = (center - p0.position).norm();
    }

    static bool CompareVertex(const Vertex& lhs, const Vertex& rhs) {
        return lhs.position.squaredNorm() < rhs.position.squaredNorm(); // assumes that the implementation uses a flat address space
    }

    void Sphere::CreateBoundingSphere(const std::vector<Vertex>& vertices) {
        if(vertices.size() == 0) {
            CreateSphere();
            return;
        }
        std::vector<Vertex> support;
        support.resize(8);

        std::vector<Vertex> buffer(vertices.begin(), vertices.end());

        //std::random_device random_device("0");
        //std::mt19937 generator(random_device());
        std::mt19937 generator(0);
        std::sort(buffer.begin(), buffer.end(), CompareVertex);
        std::shuffle(buffer.begin(), buffer.end(), generator);

        VertexList vertex_list(buffer.begin(), buffer.end());

        SmallestEnclosingSphere(support, 0, vertex_list, vertex_list.end());
    }

    bool Sphere::IsOut(const Vertex& point) {
		if ((center - point.position).norm() > radius)
			return true;
		return false;
	}

    void Sphere::SmallestEnclosingSphere(std::vector<Vertex>& support, int32_t nsp, VertexList& vertex_list, const VertexListIter& end) {
        //RK_TRACE(Core, "Start Size: {}, Total Size: {}", nsp, vertex_list.size());
        CreateSphere(support, nsp);
		if (nsp == 4)
			return;
		for (auto iterator = vertex_list.begin(); end != iterator;) {
			const auto& p = *iterator;
			if (IsOut(p)) {
                support[nsp] = p;
				SmallestEnclosingSphere(support, nsp+1, vertex_list, iterator);
				vertex_list.splice(vertex_list.begin(), vertex_list, iterator++);
			}
			else {
				iterator++;
			}
		}
        //RK_TRACE(Core, "Finish Size: {}, Total Size: {}", nsp, vertex_list.size());
    }
}
