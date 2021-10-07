#pragma once
#include "Geometry/Vertex.h"
#include "Geometry/Triangle.h"
#include "Geometry/Sphere.h"
#include "Geometry/Tetrahedra.h"

#include <vector>
#include <memory>
#include <unordered_map>

#define INIT_VERTICES_COUNT 6 // count of vertices in the initial hull
#define INIT_FACES_COUNT 8 // count of faces in the initial hull
#define VECTOR_LENGTH 1 // radius of unit sphere the dots projected into

namespace Rocket {
    class Delaunay3D {
    public:
        Delaunay3D() = default;
        ~Delaunay3D() = default;

        void Initialize(std::vector<Geometry::VertexPtr>& dots);
        void Generate();
        std::vector<Geometry::VertexPtr>& GetNodes() { return nodes; }
        std::vector<Geometry::TrianglePtr>& GetResultSurfaces() { return surfaces; }
        std::vector<Geometry::TetrahedraPtr>& GetResultTetrahedras() { return elements; }

    private:
        void MakeMesh(int addnodenum, bool iscopynodeexist);
        void MakeSupertetrahedron(double xmax, double ymax, double zmax);
        void MeshLocal(Geometry::VertexPtr& node, Geometry::Tetrahedra*& ethis);
        void StandardMethod(Geometry::VertexPtr& pnode, std::unordered_map<int32_t, Geometry::TrianglePtr>& faces);
        Geometry::Tetrahedra* FastMethod(Geometry::VertexPtr& pnode, Geometry::Tetrahedra* pethis);
        void MakeRoughMesh();
        void DeleteSupertetrahedron();
        void DeleteCreviceElement();
        void MakeFineMesh(int addnodenum);
    private:
        std::vector<Geometry::VertexPtr> nodes;
        std::vector<Geometry::TrianglePtr> surfaces;
        std::vector<Geometry::TetrahedraPtr> elements;

    public:
        int32_t method = 2;
    };

    using Delaunay3DPtr = std::shared_ptr<Delaunay3D>;
}
