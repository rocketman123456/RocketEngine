#include "Geometry/Triangle.h"
#include "Math/Parameter.h"

namespace Rocket {
    namespace Geometry {
        Triangle::Triangle() : id(GenerateId()) {
            vertices = {
                VertexPtr(new Vertex(Eigen::Vector3d::Zero())),
                VertexPtr(new Vertex(Eigen::Vector3d::Zero())),
                VertexPtr(new Vertex(Eigen::Vector3d::Zero()))
            };
            UpdateEdges();
        }

        Triangle::Triangle(const std::vector<std::shared_ptr<Vertex>>& vertices_) : id(GenerateId()) {
            assert(vertices.size() >= 3);

            vertices = {
                vertices_[0], vertices_[1], vertices_[2]
            };
            UpdateEdges();
        }

        Triangle::Triangle(const std::vector<VertexPtr>& vertices_, Tetrahedra* parent_, Tetrahedra* neighbor_) 
            : id(GenerateId()), parent(parent_), neighbor(neighbor_) {
            assert(vertices.size() >= 3);

            vertices = {
                vertices_[0], vertices_[1], vertices_[2]
            };
            UpdateEdges();
            is_active = true;
        }

        void Triangle::UpdateEdges() {
            edges = {
                EdgePtr(new Edge(vertices[0], vertices[1])),
                EdgePtr(new Edge(vertices[1], vertices[2])),
                EdgePtr(new Edge(vertices[2], vertices[0]))
            };
        }

        bool Triangle::IsRayCross(VertexPtr& sp, VertexPtr& dir) {
            Vertex v01 = {(this->vertices[1]->position) - (this->vertices[0]->position)};
            Vertex v02 = {(this->vertices[2]->position) - (this->vertices[0]->position)};
            Vertex v0g = {sp->position - (this->vertices[0]->position)};

            double det = v01.position.dot(v02.position.cross(dir->position));
            if (det > EPS) {
                double u = v0g.position.dot(v02.position.cross(dir->position)) / det;
                if (-EPS < u && u < 1.0 + EPS) {
                    double v = v01.position.dot(v0g.position.cross(dir->position)) / det;
                    if (-EPS < v && u + v < 1.0 + EPS) {
                        double t = v01.position.dot(v02.position.cross(v0g.position)) / det;
                        if (t > -EPS && t < 1.0 - EPS) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool Triangle::IsCoincidentWith(Triangle* t) {
            if(t->id == id)
                return true;
            else {
                int count = 0;
                for(auto& vertex : this->vertices) {
                    for(auto& v_other : t->vertices) {
                        if(vertex->IsCoincidentWith(v_other.get())) {
                            count++;
                        }
                    }
                }
                if(count == 3)
                    return true;
                else 
                    return false;
            }
        }

        bool Triangle::IsCoincidentWith(const VertexPtr& v) {
            return vertices[0]->IsCoincidentWith(v.get()) || vertices[1]->IsCoincidentWith(v.get()) || vertices[2]->IsCoincidentWith(v.get());
        }
    }
}
