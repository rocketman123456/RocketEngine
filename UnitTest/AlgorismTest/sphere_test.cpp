#include "Log/Log.h"
#include "Window/DesktopWindow.h"
#include "Render/SoftRasterizer.h"
#include "Render/SoftTriangle.h"

#include "Geometry/Sphere.h"

using namespace Rocket;

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <iomanip>

#include "../Utils/soft_render.h"

int main(int argc, char** argv) {
    RenderApp app;
    app.Initialize(1280, 720);

    // load image, create texture and generate mipmaps
    int32_t width = 1280;
    int32_t height = 720;
    int32_t nrChannels = 4;

    SoftRasterizer rst(width, height);
    rst.ClearAll(BufferType::COLOR | BufferType::DEPTH);

    Eigen::Vector3f eye_pos = {0.0, 0.0, 5};

    int32_t key = 0;
    int32_t frame_count = 0;

    rst.DisableWireFrame();
    //rst.EnableWireFrame();
    //rst.EnableMsaa();
    //rst.SetMsaaLevel(0);

    Geometry::Sphere sphere(Eigen::Vector3d::Zero(), 1.0);
    sphere.GenerateIcosahedronMesh(2);
    //sphere.GenerateUVMesh(10, 12);
    
    while(!app.ShouldClose()) {
        app.Tick();

        rst.NextFrame();
        rst.Clear(BufferType::COLOR | BufferType::DEPTH);
        rst.SetModel(get_model_matrix(global_angle_x, global_angle_y, global_angle_z));
        rst.SetView(get_view_matrix(eye_pos));
        rst.SetProjection(get_perspective_matrix(45, ((float)width/(float)height), 0.1, 50));
        //rst.SetProjection(get_orthographic_matrix(-6.4, 6.4, -50, 50, 3.6, -3.6));

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z

        auto& mesh = sphere.mesh;
        for(Geometry::TrianglePtr& face : mesh) {
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

        auto data = rst.FrameBuffer().data();
        app.Render(data);
    }

    app.Finalize();
    return 0;
}
