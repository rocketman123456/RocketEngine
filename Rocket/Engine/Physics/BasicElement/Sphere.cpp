#include "Physics/BasicElement/Sphere.h"

namespace Rocket {
    Sphere::Sphere(const Eigen::Vector3d& center, double radius) 
        : id_(GenerateId()), center_(center), radius_(radius) {

    }

    Sphere::Sphere(const std::vector<Vertex>& vertices) : id_(GenerateId()) {
        if(vertices.size() == 0) 
            return;
        else {
            CreateBoundingSphere(vertices);
        }
    }

    void Sphere::CreateSphere(const std::vector<Vertex>& vertices) {
        switch(vertices.size()) {
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
        center_ = Eigen::Vector3d::Zero();
        radius_ = 0.0;
    }

    void Sphere::CreateSphere(const Vertex& p1) {
        center_ = p1.Position();
        radius_ = 0.0;
    }

    void Sphere::CreateSphere(const Vertex& p1, const Vertex& p2) {
        center_ = Eigen::Vector3d::Zero();
        radius_ = 0.0;
    }

    void Sphere::CreateSphere(const Vertex& p1, const Vertex& p2, const Vertex& p3) {
        center_ = Eigen::Vector3d::Zero();
        radius_ = 0.0;
    }

    void Sphere::CreateSphere(const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4) {
        center_ = Eigen::Vector3d::Zero();
        radius_ = 0.0;
    }

    void Sphere::CreateBoundingSphere(const std::vector<Vertex>& vertices) {

    }

    void Sphere::SmallestEnclosingSphere() {

    }
}
