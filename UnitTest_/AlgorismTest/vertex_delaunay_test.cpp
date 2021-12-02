#include "Log/Log.h"
#include "Window/DesktopWindow.h"
#include "Render/SoftRasterizer.h"
#include "Render/SoftTriangle.h"

#include "Geometry/Vertex.h"
#include "Geometry/Triangle.h"
#include "Geometry/Sphere.h"
#include "Geometry/Tetrahedra.h"
#include "Geometry/MeshOperation/Delaunay3D.h"

using namespace Rocket;

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <iomanip>
#include <random>

#include "../Utils/soft_render.h"

static std::mt19937 generator(0);
static std::uniform_real_distribution<> range_distribute(0.0, 1.0);
static std::vector<Geometry::VertexPtr> nodes;
static Geometry::SpherePtr sphere;
static Geometry::TetrahedraPtr tetrahedra;
static Delaunay3DPtr delaunay_3d;

double random(double a = 0.0, double b = 1.0) {
    return range_distribute(generator) * (b-a) + a;
}

void VertexSphereTest() {
    int32_t count = 0;
    while(count <= 0) {
        std::cout << "Input node count:";
        std::cin >> count;
    }

    double radius = 1.0;
    for(int i = 0; i < count; i++) {
        double t1 = random(-3.14/2.0,3.14/2.0);
        double t2 = random(0,3.14*2.0);
        double x = radius * sin(t1) * sin(t2);
        double y = radius * sin(t1) * cos(t2);
        double z = radius * cos(t1);
        //nodes.push_back(VertexPtr(new Vertex(Eigen::Vector3d(x, y, z))));
        nodes.push_back(Geometry::VertexPtr(new Geometry::Vertex(Eigen::Vector3d(random(-1,1), random(-1,1), random(-1,1)))));
    }

    sphere = Geometry::SpherePtr(new Geometry::Sphere());
    sphere->CreateBoundingSphere(nodes);

    std::cout << "Sphere Center: " 
        << sphere->center[0] << "," 
        << sphere->center[1] << "," 
        << sphere->center[2] << std::endl;
    std::cout << "Sphere Radius: " << sphere->radius << std::endl;

    tetrahedra = Geometry::TetrahedraPtr(new Geometry::Tetrahedra());
    tetrahedra->CreateBoundingTetrahedra(
        sphere,
        Eigen::Vector3d(0,0,1),
        Eigen::Vector3d(1,0,0),
        Eigen::Vector3d(0,1,0)
    );

    delaunay_3d = Delaunay3DPtr(new Delaunay3D());
    delaunay_3d->method = 1;
    delaunay_3d->Initialize(nodes);
}

int main(int argc, char** argv) {
    //Log::Init();

    RenderApp app;

    // load image, create texture and generate mipmaps
    int32_t width = 1280;
    int32_t height = 720;
    int32_t nrChannels = 4;

    app.Initialize(width, height);

    SoftRasterizer rst(width, height);
    rst.ClearAll(BufferType::COLOR | BufferType::DEPTH);

    Eigen::Vector3f eye_pos = {0.0, 0.0, 5};

    int32_t key = 0;
    int32_t frame_count = 0;

    //rst.DisableWireFrame();
    rst.EnableWireFrame();
    rst.EnableMsaa();
    rst.SetMsaaLevel(0);

    VertexSphereTest();
    
    while(!app.ShouldClose()) {
        app.Tick();

        rst.NextFrame();
        rst.Clear(BufferType::COLOR | BufferType::DEPTH);
        rst.SetModel(get_model_matrix(global_angle_x, global_angle_y, global_angle_z));
        //rst.SetModel(get_model_matrix(global_angle_y, global_angle_z));
        //rst.SetModel(get_model_matrix(global_angle_z));
        rst.SetView(get_view_matrix(eye_pos));
        rst.SetProjection(get_perspective_matrix(45, ((float)width/(float)height), 0.1, 50));
        //rst.SetProjection(get_orthographic_matrix(-6.4, 6.4, -50, 50, 3.6, -3.6));

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z
        //rst.DrawLine3D({0.5,0.5,0}, {0.5,0.5,2}, {255,0,0}, {0,255,0});
        rst.DrawPoint3D({1,1,0}, {255,0,0});

        for(auto& node : delaunay_3d->GetNodes()) {
            rst.DrawPoint3D(Eigen::Vector3f(node->position[0], node->position[1], node->position[2]));
        }

        std::vector<Geometry::TetrahedraPtr>& meshs = delaunay_3d->GetResultTetrahedras();

        // for(TetrahedraPtr& mesh : meshs) {
        //     mesh->UpdateFaces();
        //     std::array<TrianglePtr, 4>& faces = mesh->faces;
        //     for(TrianglePtr& face : faces) {
        //         std::array<EdgePtr, 3>& edges = face->edges;
        //         for(EdgePtr& edge : edges) {
        //             rst.DrawLine3D(
        //                 Eigen::Vector3f(edge->start->position[0], edge->start->position[1], edge->start->position[2]), 
        //                 Eigen::Vector3f(edge->end->position[0], edge->end->position[1], edge->end->position[2]),
        //                 Eigen::Vector3f(255,0,0),
        //                 Eigen::Vector3f(0,0,255)
        //             );
        //         }
        //     }
        // }

        static int32_t current = 0;
        if(meshs.size() > 0) {
            current++;
            current = current % meshs.size();
            auto mesh = meshs[current];
            mesh->UpdateFaces();
            std::array<Geometry::TrianglePtr, 4>& faces = mesh->faces;
            for(Geometry::TrianglePtr& face : faces) {
                std::array<Geometry::EdgePtr, 3>& edges = face->edges;
                for(Geometry::EdgePtr& edge : edges) {
                    rst.DrawLine3D(
                        Eigen::Vector3f(edge->start->position[0], edge->start->position[1], edge->start->position[2]), 
                        Eigen::Vector3f(edge->end->position[0], edge->end->position[1], edge->end->position[2]),
                        Eigen::Vector3f(255,0,0),
                        Eigen::Vector3f(0,0,255)
                    );
                }
            }
        }
        

        // auto& faces = tetrahedra.faces;
        // for(auto face : faces) {
        //     auto& edges = face.edges;
        //     for(auto edge : edges) {
        //         rst.DrawLine3D(
        //             Eigen::Vector3f(edge.first.position[0], edge.first.position[1], edge.first.position[2]), 
        //             Eigen::Vector3f(edge.second.position[0], edge.second.position[1], edge.second.position[2]),
        //             Eigen::Vector3f(255,0,0),
        //             Eigen::Vector3f(0,0,255)
        //         );
        //     }
        // }

        auto data = rst.FrameBuffer().data();
        app.Render(data);
    }

    app.Finalize();

    //Log::End();
    return 0;
}
