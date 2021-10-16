#include "Geometry/Sphere.h"
#include "Math/Parameter.h"
#include "Log/Log.h"

#include <algorithm>
#include <cstdlib>
#include <random>
#include <iostream>

namespace Rocket {
    namespace Geometry {
        Sphere::Sphere(const Eigen::Vector3d& center, double radius) 
            : id(GenerateId()), center(center), radius(radius) {}

        Sphere::Sphere(const std::vector<VertexPtr>& vertices) : id(GenerateId()) {
            if(vertices.size() == 0) 
                return;
            else {
                CreateBoundingSphere(vertices);
            }
        }

        void Sphere::CreateSphere(const std::vector<VertexPtr>& vertices, int32_t nsp) {
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

        void Sphere::CreateSphere(const VertexPtr& p1) {
            center = p1->position;
            radius = 0.0;
        }

        void Sphere::CreateSphere(const VertexPtr& p1, const VertexPtr& p2) {
            center = (p1->position + p2->position) / 2;
            radius = ((p1->position - p2->position) / 2.0).norm();
        }

        void Sphere::CreateSphere(const VertexPtr& p0, const VertexPtr& p1, const VertexPtr& p2) {
            Eigen::Vector3d e0 = p1->position - p0->position;
            Eigen::Vector3d e1 = p2->position - p0->position;
            double a = e0.dot(e0);
            double b = e0.dot(e1);
            double c = e1.dot(e1);
            double	d = a * c - b * b;
            if(std::abs(d) < EPS)
                return;
            double s = (a - b)*c / (2 * d), t = (c - b)*a / (2 * d);
            center = p0->position + s * e0 + t * e1;
            radius = (p0->position - center).norm();
        }

        void Sphere::CreateSphere(const VertexPtr& p0, const VertexPtr& p1, const VertexPtr& p2, const VertexPtr& p3) {
            Eigen::Vector3d v1 = p1->position - p0->position;
            Eigen::Vector3d v2 = p2->position - p0->position;
            Eigen::Vector3d v3 = p3->position - p0->position;
            double V = v1.dot(v2.cross(v3));
            //Check that the three points are not on the same plane.
            if(std::abs(V) < EPS)
                return;
            V *= 2.0;
            double L1 = v1.squaredNorm();
            double L2 = v2.squaredNorm();
            double L3 = v3.squaredNorm();
            center[0] = (p0->position[0] + ((v2[1]*v3[2] - v3[1]*v2[2])*L1 - (v1[1]*v3[2] - v3[1]*v1[2])*L2 + (v1[1]*v2[2] - v2[1]*v1[2])*L3) / V);
            center[1] = (p0->position[1] + (-(v2[0]*v3[2] - v3[0]*v2[2])*L1 + (v1[0]*v3[2] - v3[0]*v1[2])*L2 - (v1[0]*v2[2] - v2[0]*v1[2])*L3) / V);
            center[2] = (p0->position[2] + ((v2[0]*v3[1] - v3[0]*v2[1])*L1 - (v1[0]*v3[1] - v3[0]*v1[1])*L2 + (v1[0]*v2[1] - v2[0]*v1[1])*L3) / V);
            radius = (center - p0->position).norm();
        }

        void Sphere::CreateBoundingSphere(const std::vector<VertexPtr>& vertices) {
            if(vertices.size() == 0) {
                CreateSphere();
                return;
            }
            std::vector<VertexPtr> support;
            support.resize(8);

            std::vector<VertexPtr> buffer(vertices.begin(), vertices.end());

            //std::random_device random_device("0");
            //std::mt19937 generator(random_device());
            std::mt19937 generator(0);
            std::sort(buffer.begin(), buffer.end(), CompareVertex);
            std::shuffle(buffer.begin(), buffer.end(), generator);

            VertexList vertex_list(buffer.begin(), buffer.end());

            SmallestEnclosingSphere(support, 0, vertex_list, vertex_list.end());
        }

        bool Sphere::IsOut(const VertexPtr& point) {
            if ((center - point->position).norm() > radius)
                return true;
            return false;
        }

        void Sphere::SmallestEnclosingSphere(std::vector<VertexPtr>& support, int32_t nsp, VertexList& vertex_list, const VertexListIter& end) {
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

        void Sphere::GenerateUVMesh(int parallels, int meridians) {
            mesh.clear();
            std::vector<Eigen::Vector3d> vertices;

            vertices.emplace_back(Eigen::Vector3d(0.0, 1.0, 0.0) * radius);
            for (uint32_t j = 0; j < parallels - 1; ++j) {
                double const polar = MATH_PI * double(j+1) / double(parallels);
                double const sp = std::sin(polar);
                double const cp = std::cos(polar);
                for (uint32_t i = 0; i < meridians; ++i) {
                    double const azimuth = 2.0 * MATH_PI * double(i) / double(meridians);
                    double const sa = std::sin(azimuth);
                    double const ca = std::cos(azimuth);
                    double const x = sp * ca;
                    double const y = cp;
                    double const z = sp * sa;
                    vertices.emplace_back(Eigen::Vector3d(x, y, z) * radius);
                }
            }
            vertices.emplace_back(Eigen::Vector3d(0.0, -1.0, 0.0) * radius);

            for (uint32_t i = 0; i < meridians; ++i) {
                uint32_t const a = i + 1;
                uint32_t const b = (i + 1) % meridians + 1;
                mesh.emplace_back(std::make_shared<Triangle>(
                    std::make_shared<Vertex>(vertices[0]), 
                    std::make_shared<Vertex>(vertices[b]), 
                    std::make_shared<Vertex>(vertices[a])));
            }

            for (uint32_t j = 0; j < parallels - 2; ++j) {
                uint32_t aStart = j * meridians + 1;
                uint32_t bStart = (j + 1) * meridians + 1;
                for (uint32_t i = 0; i < meridians; ++i) {
                    const uint32_t a = aStart + i;
                    const uint32_t a1 = aStart + (i + 1) % meridians;
                    const uint32_t b = bStart + i;
                    const uint32_t b1 = bStart + (i + 1) % meridians;
                    mesh.emplace_back(std::make_shared<Triangle>(
                        std::make_shared<Vertex>(vertices[a]), 
                        std::make_shared<Vertex>(vertices[a1]), 
                        std::make_shared<Vertex>(vertices[b1])));
                    mesh.emplace_back(std::make_shared<Triangle>(
                        std::make_shared<Vertex>(vertices[a]), 
                        std::make_shared<Vertex>(vertices[b1]), 
                        std::make_shared<Vertex>(vertices[b])));
                }
            }

            for (uint32_t i = 0; i < meridians; ++i) {
                uint32_t const a = i + meridians * (parallels - 2) + 1;
                uint32_t const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
                mesh.emplace_back(std::make_shared<Triangle>(
                    std::make_shared<Vertex>(vertices[vertices.size() - 1]), 
                    std::make_shared<Vertex>(vertices[b]), 
                    std::make_shared<Vertex>(vertices[a])));
            }
        }

        static Eigen::Vector3d GetMiddlePoint(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2) {
            Eigen::Vector3d middle = (v1 + v2)/2;
            middle = middle.normalized();
            return middle;
        }

        void Sphere::GenerateIcosahedronMesh(int recursionLevel) {
            const double t = (1.0 + std::sqrt(5.0)) / 2.0;

            std::vector<Eigen::Vector3d> vertices;

            // Vertices
            vertices.emplace_back(Eigen::Vector3d(-1.0,  t, 0.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d( 1.0,  t, 0.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(-1.0, -t, 0.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d( 1.0, -t, 0.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(0.0, -1.0,  t).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(0.0,  1.0,  t).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(0.0, -1.0, -t).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(0.0,  1.0, -t).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d( t, 0.0, -1.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d( t, 0.0,  1.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(-t, 0.0, -1.0).normalized() * radius);
            vertices.emplace_back(Eigen::Vector3d(-t, 0.0,  1.0).normalized() * radius);

            // Faces
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[0]), std::make_shared<Vertex>(vertices[11]), std::make_shared<Vertex>(vertices[5])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[0]), std::make_shared<Vertex>(vertices[5]), std::make_shared<Vertex>(vertices[1])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[0]), std::make_shared<Vertex>(vertices[1]), std::make_shared<Vertex>(vertices[7])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[0]), std::make_shared<Vertex>(vertices[7]), std::make_shared<Vertex>(vertices[10])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[0]), std::make_shared<Vertex>(vertices[10]), std::make_shared<Vertex>(vertices[11])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[1]), std::make_shared<Vertex>(vertices[5]), std::make_shared<Vertex>(vertices[9])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[5]), std::make_shared<Vertex>(vertices[11]), std::make_shared<Vertex>(vertices[4])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[11]), std::make_shared<Vertex>(vertices[10]), std::make_shared<Vertex>(vertices[2])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[10]), std::make_shared<Vertex>(vertices[7]), std::make_shared<Vertex>(vertices[6])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[7]), std::make_shared<Vertex>(vertices[1]), std::make_shared<Vertex>(vertices[8])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[3]), std::make_shared<Vertex>(vertices[9]), std::make_shared<Vertex>(vertices[4])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[3]), std::make_shared<Vertex>(vertices[4]), std::make_shared<Vertex>(vertices[2])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[3]), std::make_shared<Vertex>(vertices[2]), std::make_shared<Vertex>(vertices[6])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[3]), std::make_shared<Vertex>(vertices[6]), std::make_shared<Vertex>(vertices[8])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[3]), std::make_shared<Vertex>(vertices[8]), std::make_shared<Vertex>(vertices[9])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[4]), std::make_shared<Vertex>(vertices[9]), std::make_shared<Vertex>(vertices[5])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[2]), std::make_shared<Vertex>(vertices[4]), std::make_shared<Vertex>(vertices[11])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[6]), std::make_shared<Vertex>(vertices[2]), std::make_shared<Vertex>(vertices[10])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[8]), std::make_shared<Vertex>(vertices[6]), std::make_shared<Vertex>(vertices[7])));
            mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(vertices[9]), std::make_shared<Vertex>(vertices[8]), std::make_shared<Vertex>(vertices[1])));
        
            // refine triangles
            for (int i = 0; i < recursionLevel; i++) {
                std::vector<TrianglePtr> temp_mesh;
                for(auto tri : mesh) {
                    // replace triangle by 4 triangles
                    auto a = GetMiddlePoint(tri->vertices[0]->position, tri->vertices[1]->position) * radius;
                    auto b = GetMiddlePoint(tri->vertices[1]->position, tri->vertices[2]->position) * radius;
                    auto c = GetMiddlePoint(tri->vertices[2]->position, tri->vertices[0]->position) * radius;

                    temp_mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(tri->vertices[0]->position), std::make_shared<Vertex>(a), std::make_shared<Vertex>(c)));
                    temp_mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(tri->vertices[1]->position), std::make_shared<Vertex>(b), std::make_shared<Vertex>(a)));
                    temp_mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(tri->vertices[2]->position), std::make_shared<Vertex>(c), std::make_shared<Vertex>(b)));
                    temp_mesh.emplace_back(std::make_shared<Triangle>(std::make_shared<Vertex>(a), std::make_shared<Vertex>(b), std::make_shared<Vertex>(c)));
                }
                mesh = temp_mesh;
            }
        }
        
    }// namespace Geometry
}// namespace Rocket
