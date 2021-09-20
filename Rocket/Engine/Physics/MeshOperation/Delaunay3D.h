#pragma once
#include "Physics/BasicElement/Vertex.h"
#include "Physics/BasicElement/Triangle.h"
#include "Physics/BasicElement/Sphere.h"
#include "Physics/BasicElement/Tetrahedra.h"

#include <vector>
#include <memory>

#define INIT_VERTICES_COUNT 6 // count of vertices in the initial hull
#define INIT_FACES_COUNT 8 // count of faces in the initial hull
#define VECTOR_LENGTH 1 // radius of unit sphere the dots projected into

namespace Rocket {
    class Delaunay3D {
    public:
        Delaunay3D() = default;
        ~Delaunay3D() = default;

        void Initialize(std::vector<Vertex>& dots);
        void Generate();
        std::vector<Vertex>& GetNodes() { return nodes; }
        std::vector<Triangle>& GetResultSurfaces() { return surfaces; }
        std::vector<std::shared_ptr<Tetrahedra>>& GetResultTetrahedras() { return elements; }

    private:
        void MakeMesh(int addnodenum, bool iscopynodeexist);
        void MakeSupertetrahedron(double xmax, double ymax, double zmax);
        void MeshLocal(Vertex* node, std::shared_ptr<Tetrahedra>& ethis);
        void MakeRoughMesh();
        void DeleteSupertetrahedron();
        void DeleteCreviceElement();
        void MakeFineMesh(int addnodenum);
    private:
        std::vector<Vertex> nodes;
        std::vector<Triangle> surfaces;
        std::vector<std::shared_ptr<Tetrahedra>> elements;
    };
}
