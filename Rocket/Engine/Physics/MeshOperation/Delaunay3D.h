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

        void Initialize(std::vector<VertexPtr>& dots);
        void Generate();
        std::vector<VertexPtr>& GetNodes() { return nodes; }
        std::vector<TrianglePtr>& GetResultSurfaces() { return surfaces; }
        std::vector<TetrahedraPtr>& GetResultTetrahedras() { return elements; }

    private:
        void MakeMesh(int addnodenum, bool iscopynodeexist);
        void MakeSupertetrahedron(double xmax, double ymax, double zmax);
        void MeshLocal(VertexPtr& node, Tetrahedra*& ethis);
        void MakeRoughMesh();
        void DeleteSupertetrahedron();
        void DeleteCreviceElement();
        void MakeFineMesh(int addnodenum);
    private:
        std::vector<VertexPtr> nodes;
        std::vector<TrianglePtr> surfaces;
        std::vector<TetrahedraPtr> elements;
    };

    using Delaunay3DPtr = std::shared_ptr<Delaunay3D>;
}
