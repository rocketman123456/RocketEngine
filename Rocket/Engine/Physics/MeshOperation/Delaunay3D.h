#pragma once
#include "Physics/Basic/Vertex.h"
#include "Physics/Basic/Triangle.h"
#include "Physics/Basic/Sphere.h"
#include "Physics/Basic/Tetrahedra.h"

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

        void Initialize(std::vector<VertexPtr>& dots);
        void Generate();
        std::vector<VertexPtr>& GetNodes() { return nodes; }
        std::vector<TrianglePtr>& GetResultSurfaces() { return surfaces; }
        std::vector<TetrahedraPtr>& GetResultTetrahedras() { return elements; }

    private:
        void MakeMesh(int addnodenum, bool iscopynodeexist);
        void MakeSupertetrahedron(double xmax, double ymax, double zmax);
        void MeshLocal(VertexPtr& node, Tetrahedra*& ethis);
        void StandardMethod(VertexPtr& pnode, std::unordered_map<int32_t, TrianglePtr>& faces);
        Tetrahedra* FastMethod(VertexPtr& pnode, Tetrahedra* pethis);
        void MakeRoughMesh();
        void DeleteSupertetrahedron();
        void DeleteCreviceElement();
        void MakeFineMesh(int addnodenum);
    private:
        std::vector<VertexPtr> nodes;
        std::vector<TrianglePtr> surfaces;
        std::vector<TetrahedraPtr> elements;

    public:
        int32_t method = 2;
    };

    using Delaunay3DPtr = std::shared_ptr<Delaunay3D>;
}
