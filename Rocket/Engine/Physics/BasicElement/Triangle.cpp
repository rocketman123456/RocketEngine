#include "Physics/BasicElement/Triangle.h"
#include "Physics/BasicElement/Parameter.h"

namespace Rocket {
    Triangle::Triangle() : id(GenerateId()) {
        vertices = {
            Vertex(Eigen::Vector3d::Zero()),
            Vertex(Eigen::Vector3d::Zero()),
            Vertex(Eigen::Vector3d::Zero())
        };
        edges = {
            std::make_pair(vertices[0], vertices[1]),
            std::make_pair(vertices[1], vertices[2]),
            std::make_pair(vertices[2], vertices[0])
        };
    }

    Triangle::Triangle(const std::vector<Vertex>& vertices_) : id(GenerateId()) {
        assert(vertices.size() >= 3);

        vertices = {
            vertices_[0], vertices_[1], vertices_[2]
        };
        edges = {
            std::make_pair(vertices[0], vertices[1]),
            std::make_pair(vertices[1], vertices[2]),
            std::make_pair(vertices[2], vertices[0])
        };
    }

    Triangle::Triangle(const std::vector<Vertex>& vertices_, 
        std::shared_ptr<Tetrahedra>& parent, std::shared_ptr<Tetrahedra>& neighbor) : id(GenerateId()) {
        assert(vertices.size() >= 3);

        vertices = {
            vertices_[0], vertices_[1], vertices_[2]
        };
        edges = {
            std::make_pair(vertices[0], vertices[1]),
            std::make_pair(vertices[1], vertices[2]),
            std::make_pair(vertices[2], vertices[0])
        };

        is_active = true;
    }

    void Triangle::UpdateFaces() {
        edges = {
            std::make_pair(vertices[0], vertices[1]),
            std::make_pair(vertices[1], vertices[2]),
            std::make_pair(vertices[2], vertices[0])
        };
    }

    bool Triangle::operator==(const Triangle& t) {
        if(t.id == id)
            return true;
        else if ((this->vertices[0] == t.vertices[0] && 
                this->vertices[1] == t.vertices[2] && 
                this->vertices[2] == t.vertices[1]) ||
			    (this->vertices[0] == t.vertices[1] && 
                this->vertices[1] == t.vertices[0] && 
                this->vertices[2] == t.vertices[2]) ||
			    (this->vertices[0] == t.vertices[2] && 
                this->vertices[1] == t.vertices[1] && 
                this->vertices[2] == t.vertices[0])) {
			return true;
		}
        else
            return false;
    }

    bool Triangle::IsRayCross(Vertex sp, Vertex dir) {
        Vertex v01 = {(this->vertices[1].position) - (this->vertices[0].position)};
		Vertex v02 = {(this->vertices[2].position) - (this->vertices[0].position)};
		Vertex v0g = {sp.position - (this->vertices[0].position)};

		double det = v01.position.dot(v02.position.cross(dir.position));
		if (det > EPS) {
			double u = v0g.position.dot(v02.position.cross(dir.position)) / det;
			if (-EPS < u && u < 1.0 + EPS) {
				double v = v01.position.dot(v0g.position.cross(dir.position)) / det;
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

    bool Triangle::IsCoincidentWith(const Vertex& v) {
        return vertices[0].IsCoincidentWith(v) || vertices[1].IsCoincidentWith(v) || vertices[2].IsCoincidentWith(v);
    }
}
